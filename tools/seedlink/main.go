package main

import (
	"context"
	"fmt"
	"log"
	"os"
	"os/signal"
	"sync"
	"syscall"

	"github.com/anyshake/observer/pkg/ringbuf"
	"github.com/anyshake/observer/pkg/transport"
	"github.com/bclswl0827/slgo"
	cmap "github.com/orcaman/concurrent-map/v2"
	messagebus "github.com/vardius/message-bus"
)

func main() {
	ctx, stop := signal.NotifyContext(context.Background(), os.Interrupt, syscall.SIGTERM)
	defer stop()

	args := parseCommandLine()
	messageBus := messagebus.New(65535)
	ringBuf := ringbuf.New[*channelData](RINGBUF_SIZE)

	// Connect to hardware device with serial transport
	tr, err := transport.New(fmt.Sprintf("serial://%s?baudrate=%d", args.device, args.baudrate), 0)
	if err != nil {
		log.Fatalln(err)
	}
	if err := tr.Open(); err != nil {
		log.Fatalln(err)
	}
	defer tr.Close()
	log.Printf("connected to hardware device %s, baudrate %d", args.device, args.baudrate)

	hw := hardware{
		messageBus: messageBus,
		transport:  tr,
		ringBuf:    &ringBuf,
	}
	var wg sync.WaitGroup
	wg.Go(func() {
		if err := hw.Start(ctx); err != nil {
			log.Printf("error occurred in hardware: %v", err)
			stop()
		}
	})

	// Create and start SeedLink server instance
	seedLinkServer := slgo.New(
		&provider{&hw},
		&consumer{
			messageBus:  messageBus,
			subscribers: cmap.New[eventHandler](),
		},
		&hooks{},
	)
	log.Printf("starting seedlink server on %s:%d", args.listenAddr, args.listenPort)
	if err := seedLinkServer.Start(ctx, args.listenAddr, args.listenPort, true); err != nil {
		log.Fatalln(err)
	}

	log.Println("seedlink server has been stopped, exiting...")
	wg.Wait()
}
