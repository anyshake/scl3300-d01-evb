package main

import (
	"context"
	"log"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/anyshake/observer/pkg/ringbuf"
	"github.com/anyshake/observer/pkg/transport"
	"github.com/samber/lo"
	messagebus "github.com/vardius/message-bus"
)

type hardware struct {
	messageBus messagebus.MessageBus
	transport  transport.ITransport
	ringBuf    *ringbuf.Buffer[*channelData]

	mu           sync.Mutex
	startTime    time.Time
	updatedAt    time.Time
	stationCode  string
	networkCode  string
	locationCode string
	channels     []string

	xBuf []int32
	yBuf []int32
	zBuf []int32
}

func (h *hardware) GetStartTime() time.Time {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.startTime
}

func (h *hardware) GetUpdatedAt() time.Time {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.updatedAt
}

func (h *hardware) GetChannels() []string {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.channels
}

func (h *hardware) GetLocationCode() string {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.locationCode
}

func (h *hardware) GetNetworkCode() string {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.networkCode
}

func (h *hardware) GetStationCode() string {
	h.mu.Lock()
	defer h.mu.Unlock()
	return h.stationCode
}

func (h *hardware) Start(ctx context.Context) error {
	if err := h.transport.Flush(); err != nil {
		return err
	}

	h.mu.Lock()
	h.stationCode = "TEST"
	h.locationCode = "00"
	h.networkCode = "SS"
	h.channels = []string{"ENE", "ENN", "ENZ"}
	h.mu.Unlock()

	for {
		select {
		case <-ctx.Done():
			return nil
		default:
		}

		line, timeout, _, err := h.transport.ReadUntil(ctx, 512, func(buf *[]byte, updatedAt *time.Time) bool {
			return strings.HasSuffix(string(*buf), "\n")
		}, time.Second)
		if timeout {
			continue
		}
		if err != nil {
			return err
		}

		data := strings.Split(string(line), ",")
		if len(data) < 16 {
			continue
		}

		G := 978.046
		h.xBuf = append(h.xBuf, lo.Map(data[:5], func(n string, _ int) int32 {
			v, _ := strconv.Atoi(n)
			return int32(float64(v) / 12000.0 * G * 1000)
		})...)
		h.yBuf = append(h.yBuf, lo.Map(data[5:10], func(n string, _ int) int32 {
			v, _ := strconv.Atoi(n)
			return int32(float64(v) / 12000.0 * G * 1000)
		})...)
		h.zBuf = append(h.zBuf, lo.Map(data[10:15], func(n string, _ int) int32 {
			v, _ := strconv.Atoi(n)
			return int32(float64(v) / 12000.0 * G * 1000)
		})...)
		sampleRate, _ := strconv.Atoi(strings.TrimSpace(data[15]))

		if len(h.zBuf) == sampleRate {
			currentTime := time.Now()
			chData := &channelData{
				sampleRate: sampleRate,
				timestamp:  currentTime,
				channelMap: map[string][]int32{
					"ENE": h.xBuf,
					"ENN": h.yBuf,
					"ENZ": h.zBuf,
				},
			}
			h.xBuf = []int32{}
			h.yBuf = []int32{}
			h.zBuf = []int32{}
			h.ringBuf.Push(chData)
			h.messageBus.Publish(MSGBUS_TOPIC, chData)

			h.mu.Lock()
			h.updatedAt = currentTime
			if h.startTime.IsZero() {
				h.startTime = currentTime
			}
			h.mu.Unlock()
			log.Println("received 1 message", currentTime.UnixMilli())
		} else if len(h.zBuf) > sampleRate {
			h.xBuf = []int32{}
			h.yBuf = []int32{}
			h.zBuf = []int32{}
			h.ringBuf.Reset()
		}
	}
}
