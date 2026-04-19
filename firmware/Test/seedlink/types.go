package main

import "time"

const (
	RINGBUF_SIZE = 3600
	MSGBUS_TOPIC = "seedlink"
)

const (
	SOFTWARE     = "scl3300-seedlink"
	ORGANIZATION = "anyshake.org"
	DESCRIPTION  = "AnyShake Project EVB"
)

type channelData struct {
	sampleRate int
	timestamp  time.Time
	channelMap map[string][]int32
}

type eventHandler = func(data *channelData)
