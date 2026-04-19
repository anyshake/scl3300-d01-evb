package main

import (
	"time"

	"github.com/bclswl0827/slgo/handlers"
	"github.com/samber/lo"
)

type provider struct {
	hw *hardware
}

func (p *provider) GetSoftware() string {
	return SOFTWARE
}

func (p *provider) GetStartTime() time.Time {
	return p.hw.GetStartTime().UTC()
}

func (p *provider) GetCurrentTime() time.Time {
	return p.hw.GetUpdatedAt().UTC()
}

func (p *provider) GetOrganization() string {
	return ORGANIZATION
}

func (p *provider) GetStations() []handlers.SeedLinkStation {
	return []handlers.SeedLinkStation{
		{
			BeginSequence: "000000",
			EndSequence:   "FFFFFF",
			Station:       p.hw.GetStationCode(),
			Network:       p.hw.GetNetworkCode(),
			Description:   DESCRIPTION,
		},
	}
}

func (p *provider) GetStreams() []handlers.SeedLinkStream {
	return lo.Map(p.hw.GetChannels(), func(channel string, _ int) handlers.SeedLinkStream {
		return handlers.SeedLinkStream{
			BeginTime: p.GetStartTime().Format("2006-01-02 15:04:01"),
			EndTime:   p.GetCurrentTime().Format("2006-01-02 15:04:01"),
			Location:  p.hw.GetLocationCode(),
			Station:   p.hw.GetStationCode(),
			SeedName:  channel,
			Type:      "D",
		}
	})
}

func (p *provider) GetCapabilities() []handlers.SeedLinkCapability {
	return []handlers.SeedLinkCapability{
		{Name: "info:all"}, {Name: "info:gaps"}, {Name: "info:streams"},
		{Name: "dialup"}, {Name: "info:id"}, {Name: "multistation"},
		{Name: "window-extraction"}, {Name: "info:connections"},
		{Name: "info:capabilities"}, {Name: "info:stations"},
	}
}

func (p *provider) QueryHistory(startTime, endTime time.Time, channels []handlers.SeedLinkChannel) ([]handlers.SeedLinkDataPacket, error) {
	var dataPackets []handlers.SeedLinkDataPacket

	for _, dataObj := range p.hw.ringBuf.Values() {
		if dataObj.timestamp.Before(startTime) || dataObj.timestamp.After(endTime) {
			continue
		}

		for _, channel := range channels {
			dataArr, ok := dataObj.channelMap[channel.ChannelName]
			if !ok {
				continue
			}
			dataPackets = append(dataPackets, handlers.SeedLinkDataPacket{
				Timestamp:  dataObj.timestamp.UnixMilli(),
				SampleRate: dataObj.sampleRate,
				Channel:    channel.ChannelName,
				DataArr:    dataArr,
			})
		}
	}

	return dataPackets, nil
}
