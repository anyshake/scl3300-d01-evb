package main

import (
	"flag"
)

type arguments struct {
	device     string
	baudrate   int
	listenAddr string
	listenPort int
}

func parseCommandLine() (args arguments) {
	flag.StringVar(&args.device, "device", "/dev/ttyUSB0", "Hardware device to use")
	flag.IntVar(&args.baudrate, "baudrate", 115200, "Baudrate for the device")
	flag.StringVar(&args.listenAddr, "addr", "0.0.0.0", "Listen address for SeedLink server")
	flag.IntVar(&args.listenPort, "port", 18000, "Listen port for SeedLink server")

	flag.Parse()
	return args
}
