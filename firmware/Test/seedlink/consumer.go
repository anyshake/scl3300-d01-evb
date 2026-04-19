package main

import (
	"fmt"

	"github.com/bclswl0827/slgo/handlers"
	cmap "github.com/orcaman/concurrent-map/v2"
	messagebus "github.com/vardius/message-bus"
)

type consumer struct {
	messageBus  messagebus.MessageBus
	subscribers cmap.ConcurrentMap[string, eventHandler]
}

func (c *consumer) Subscribe(clientId string, channels []handlers.SeedLinkChannel, eventHandler func(handlers.SeedLinkDataPacket)) error {
	if _, ok := c.subscribers.Get(clientId); ok {
		return fmt.Errorf("client %s has already subscribed", clientId)
	}
	handler := func(data *channelData) {
		for _, channel := range channels {
			dataArr, ok := data.channelMap[channel.ChannelName]
			if !ok {
				continue
			}
			eventHandler(handlers.SeedLinkDataPacket{
				Timestamp:  data.timestamp.UnixMilli(),
				SampleRate: data.sampleRate,
				Channel:    channel.ChannelName,
				DataArr:    dataArr,
			})
		}
	}
	c.subscribers.Set(clientId, handler)
	c.messageBus.Subscribe(MSGBUS_TOPIC, handler)
	return nil
}

func (c *consumer) Unsubscribe(clientId string) error {
	fn, ok := c.subscribers.Get(clientId)
	if !ok {
		return fmt.Errorf("client %s has not subscribed", clientId)
	}
	c.messageBus.Unsubscribe(MSGBUS_TOPIC, fn)
	c.subscribers.Remove(clientId)
	return nil
}
