#include "User/Inc/packet.h"

uint16_t get_data_packet_size(uint8_t channel_chunk_length) {
    uint16_t base_size = sizeof(packet_header_t) +
                         sizeof(packet_timestamp_t) +
                         sizeof(packet_device_config_t) +
                         sizeof(packet_variable_data_t) +
                         sizeof(packet_checksum_t) +
                         sizeof(packet_trailer_t);
    base_size += (sizeof(int16_t) * channel_chunk_length * 3);

    return base_size;
}

void encode_data_packet(uint16_t packet_sending_interval,
                        uint16_t sample_rate,
                        uint8_t uart_buf[],
                        uint16_t uart_buf_size,
                        int16_t adc1_ch1[],
                        int16_t adc1_ch2[],
                        int16_t adc1_ch3[],
                        uint8_t channel_chunk_length,
                        int64_t timestamp,
                        bool gnss_enabled) {
    uint16_t offset = 0;
    uint8_t* bytes;

    uart_buf[0] = 0x01;
    uart_buf[1] = 0xFE;
    uart_buf[uart_buf_size - 2] = 0xEF;
    uart_buf[uart_buf_size - 1] = 0x10;

    bytes = (uint8_t*)&timestamp;
    for (uint8_t i = 0; i < sizeof(packet_timestamp_t); i++) {
        offset = sizeof(packet_header_t) + i;
        uart_buf[offset] = bytes[i];
    }

    uint32_t device_config = 0x00;
    switch (packet_sending_interval) {
        case 100:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_100MS << 30;
            break;
        case 200:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_200MS << 30;
            break;
        case 500:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_500MS << 30;
            break;
        case 1000:
            device_config |= PACKET_DEVICE_CONFIG_PACKET_INTERVAL_1000MS << 30;
            break;
    }
    switch (sample_rate) {
        case 10:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_10HZ << 27;
            break;
        case 20:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_20HZ << 27;
            break;
        case 50:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_50HZ << 27;
            break;
        case 100:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_100HZ << 27;
            break;
        case 200:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_200HZ << 27;
            break;
        case 250:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_250HZ << 27;
            break;
        case 500:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_500HZ << 27;
            break;
        case 1000:
            device_config |= PACKET_DEVICE_CONFIG_SAMPLE_RATE_1000HZ << 27;
            break;
    }
    device_config |= (gnss_enabled ? PACKET_DEVICE_CONFIG_GNSS_AVAILABLE : PACKET_DEVICE_CONFIG_GNSS_NOT_AVAILABLE) << 26;
    device_config |= (PACKET_DEVICE_CONFIG_CHANNEL_4_INT16 << 18) |
                     (PACKET_DEVICE_CONFIG_CHANNEL_5_INT16 << 16) |
                     (PACKET_DEVICE_CONFIG_CHANNEL_6_INT16 << 14);
    device_config |= ((gnss_enabled ? PACKET_VARIABLE_CONFIG_BIT_3_ENABLED : PACKET_VARIABLE_CONFIG_BIT_3_DISABLED) << 3) |
                     ((gnss_enabled ? PACKET_VARIABLE_CONFIG_BIT_2_ENABLED : PACKET_VARIABLE_CONFIG_BIT_2_DISABLED) << 2) |
                     ((gnss_enabled ? PACKET_VARIABLE_CONFIG_BIT_1_ENABLED : PACKET_VARIABLE_CONFIG_BIT_1_DISABLED) << 1) |
                     (PACKET_VARIABLE_CONFIG_BIT_0_ENABLED);
    bytes = (uint8_t*)&device_config;
    for (uint8_t i = 0; i < sizeof(packet_device_config_t); i++) {
        offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + i;
        uart_buf[offset] = bytes[i];
    }

    uint32_t device_id = 0xFFFFFFFF;
    float tempFloat = 0;
    switch ((timestamp / 1000) % 10) {
        case 0:
            bytes = (uint8_t*)&device_id;
            break;
        case 1:
            bytes = (uint8_t*)&tempFloat;
            break;
        case 2:
            bytes = (uint8_t*)&tempFloat;
            break;
        case 3:
            bytes = (uint8_t*)&tempFloat;
            break;
    }
    for (uint8_t i = 0; i < sizeof(packet_variable_data_t); i++) {
        offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + sizeof(packet_device_config_t) + i;
        uart_buf[offset] = bytes[i];
    }

    offset = sizeof(packet_header_t) + sizeof(packet_timestamp_t) + sizeof(packet_device_config_t) + sizeof(packet_variable_data_t);
    for (uint8_t i = 0; i < channel_chunk_length; i++) {
        uint16_t adc_offset = offset + i * sizeof(int16_t);
        for (uint8_t j = 0; j < sizeof(int16_t); j++) {
            bytes = (uint8_t*)&adc1_ch1[i];
            uart_buf[adc_offset + j] = bytes[j];
            bytes = (uint8_t*)&adc1_ch2[i];
            uart_buf[adc_offset + j + 1 * channel_chunk_length * sizeof(int16_t)] = bytes[j];
            bytes = (uint8_t*)&adc1_ch3[i];
            uart_buf[adc_offset + j + 2 * channel_chunk_length * sizeof(int16_t)] = bytes[j];
        }
    }

    offset = uart_buf_size - sizeof(packet_trailer_t) - sizeof(packet_checksum_t);
    uint8_t xor_checksum = 0;
    for (uint16_t i = sizeof(packet_header_t); i < offset; i++) {
        xor_checksum ^= uart_buf[i];
    }
    uart_buf[offset] = xor_checksum;
}
