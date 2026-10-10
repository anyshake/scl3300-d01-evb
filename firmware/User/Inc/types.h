#ifndef __TYPES_H
#define __TYPES_H

#include <stdbool.h>
#include <stdint.h>

#include "cmsis_os2.h"
#include "User/Inc/gnss/pps.h"
#include "User/Inc/settings.h"

typedef struct {
    float latitude;
    float longitude;
    float elevation;
    bool valid;
} packet_gnss_location_t;

typedef struct {
    uint32_t sequence;
    int64_t timestamp;
    int16_t accel_data[3];  /* Protocol channels 4, 5, 6: Z, X, Y. */
} scl3300_acquisition_message_t;

typedef struct {
    uint32_t baud_rate;
    uint16_t sample_rate;
    uint16_t packet_sending_interval;
    uint8_t channel_chunk_length;
    bool use_gnss_time;

    gnss_pps_state_t gnss_pps_state;
    volatile uint64_t monotonic_time_us;
    volatile uint32_t actual_freq_hz;
    volatile float current_ppm;
    bool gnss_time_locked;
    uint8_t gnss_satellites;
    float gnss_hdop;
    packet_gnss_location_t gnss_location;

    int16_t accel_acquisition_channel_buffer[3][CHANNEL_SAMPLE_CAPACITY];
    uint8_t uart1_packet_buffer[2 + 8 + 4 + 4 +
                                3 * CHANNEL_SAMPLE_CAPACITY * sizeof(int16_t) + 1 + 2];
    osMessageQueueId_t sensor_acquisition_queue;
    volatile uint32_t sensor_queue_overflow_count;
    osThreadId_t gnss_acquire_task_handle;
    osThreadId_t sensor_acquire_task_handle;
} scl3300_global_states_t;

enum scl3300_thread_flags_t {
    GNSS_1PPS_EDGE = 0x01,
    GNSS_1PPS_UPDATED = 0x02,
    SENSOR_ACQUIRE_ACT = 0x04,
};

#endif
