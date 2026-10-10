#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Core/Inc/dma.h"
#include "Core/Inc/main.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "task.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/utils.h"
#include "User/Inc/packet.h"
#include "User/Inc/peripheral.h"
#include "User/Inc/reader.h"
#include "User/Inc/settings.h"
#include "User/Inc/types.h"
#include "User/Inc/utils.h"

#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/spi.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uart2.h"
#include "Utils/Inc/uptime.h"
#include "Utils/Inc/systime.h"

static uint8_t gnss_acquire_task_stack[1536];
static StaticTask_t gnss_acquire_task_cb;
static uint8_t send_packet_task_stack[1024];
static StaticTask_t send_packet_task_cb;
static uint8_t sensor_acquire_task_stack[1024];
static StaticTask_t sensor_acquire_task_cb;
static uint8_t sensor_acquisition_queue_mem[SENSOR_ACQUISITION_QUEUE_DEPTH *
                                             sizeof(scl3300_acquisition_message_t)];
static StaticQueue_t sensor_acquisition_queue_cb;

/* HAL's EXTI callback has no user argument; this is its sole state reference. */
static scl3300_global_states_t* irq_states;

static void handle_incoming_time_sync_data(uint64_t monotonic_time_us,
                                           uint32_t actual_freq_hz) {
    scl3300_global_states_t* states = irq_states;
    if (states == NULL || actual_freq_hz == 0) {
        return;
    }

    mcu_utils_uart2_flush();
    mcu_utils_systime_calib_freq(actual_freq_hz);

    uint32_t nominal_freq_hz = mcu_utils_systime_get_nominal_freq_hz();
    int64_t freq_diff_hz = (int64_t)actual_freq_hz - nominal_freq_hz;
    states->monotonic_time_us = monotonic_time_us;
    states->actual_freq_hz = actual_freq_hz;
    states->current_ppm = ((float)freq_diff_hz * 1.0e6f) / nominal_freq_hz;

    if (states->gnss_acquire_task_handle != NULL) {
        osThreadFlagsSet(states->gnss_acquire_task_handle, GNSS_1PPS_UPDATED);
    }
}

void EXTI9_5_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void HAL_GPIO_EXTI_Callback(uint16_t pin) {
    if (pin == GPIO_PIN_8 && irq_states != NULL && irq_states->use_gnss_time) {
        gnss_pps_handle_timer_interrupt(&irq_states->gnss_pps_state,
                                       handle_incoming_time_sync_data);
        if (irq_states->gnss_acquire_task_handle != NULL) {
            osThreadFlagsSet(irq_states->gnss_acquire_task_handle, GNSS_1PPS_EDGE);
        }
    }
}

void task_gnss_acquire(void* argument) {
    scl3300_global_states_t* states = (scl3300_global_states_t*)argument;
    bool sampling_started = false;
    bool has_valid_offset = false;
    uint8_t consecutive_valid_count = 0;
    int64_t previous_offset_us = INT64_MAX;
    uint8_t message_buf[GNSS_SENTENCE_BUFFER_SIZE];

    display_gnss_pps(0, false, 0.0f);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    for (;;) {
        uint32_t flags = osThreadFlagsWait(GNSS_1PPS_EDGE | GNSS_1PPS_UPDATED,
                                           osFlagsWaitAny, 1500);
        if ((flags & osFlagsError) != 0 ||
            (flags & (GNSS_1PPS_EDGE | GNSS_1PPS_UPDATED)) == 0) {
            continue;
        }

        __disable_irq();
        uint8_t valid_pps_count = states->gnss_pps_state.valid_pps_count;
        uint64_t monotonic_time_us = states->monotonic_time_us;
        uint32_t actual_freq_hz = states->actual_freq_hz;
        float current_ppm = states->current_ppm;
        __enable_irq();

        bool calibrated = actual_freq_hz != 0;
        if (flags & GNSS_1PPS_UPDATED) {
            if (mcu_utils_systime_get_uptime_us() - (int64_t)monotonic_time_us >=
                500 * 1000) {
                continue;
            }

            gnss_status_t gnss_status = {.hdop = 999.0f};
            gnss_location_t gnss_location = {0};
            gnss_time_t gnss_time = {0};
            int64_t current_offset_us = INT64_MAX;
            bool parsed = parse_gnss_message(message_buf, &gnss_status,
                                             &gnss_location, &gnss_time,
                                             (int64_t)monotonic_time_us,
                                             &current_offset_us, true);
            if (!parsed) {
                if (!sampling_started) {
                    display_gnss_pps(valid_pps_count, calibrated, current_ppm);
                }
                consecutive_valid_count = 0;
                has_valid_offset = false;
                continue;
            }

            mcu_utils_led_blink(MCU_STATE_PIN, 1, true);
            mcu_utils_gpio_high(MCU_STATE_PIN);

            if (!sampling_started) {
                display_gnss_acquiring(gnss_status.satellites,
                                       gnss_status.hdop, current_ppm);
            }

            bool acceptable_fix = gnss_time.is_valid &&
                                  gnss_status.satellites > 0 &&
                                  gnss_location.is_valid &&
                                  gnss_status.hdop <= GNSS_REQUIRED_VALID_HDOP;
            if (acceptable_fix) {
                if (!has_valid_offset || current_offset_us == previous_offset_us) {
                    if (consecutive_valid_count < 10) {
                        consecutive_valid_count++;
                    }
                } else {
                    consecutive_valid_count = 1;
                }
                previous_offset_us = current_offset_us;
                has_valid_offset = true;

                if (consecutive_valid_count >= 10) {
                    mcu_utils_systime_set_offset(current_offset_us);
                    taskENTER_CRITICAL();
                    states->gnss_satellites = gnss_status.satellites;
                    states->gnss_hdop = gnss_status.hdop;
                    states->gnss_location.latitude = gnss_location.latitude;
                    states->gnss_location.longitude = gnss_location.longitude;
                    states->gnss_location.elevation = gnss_location.elevation;
                    states->gnss_location.valid = true;
                    states->gnss_time_locked = true;
                    taskEXIT_CRITICAL();
                    if (!sampling_started) {
                        sampling_started = true;
                        display_gnss_locked(gnss_status.satellites,
                                            gnss_status.hdop, current_ppm);
                        osThreadFlagsSet(states->sensor_acquire_task_handle,
                                         SENSOR_ACQUIRE_ACT);
                    }
                }
            } else {
                consecutive_valid_count = 0;
                has_valid_offset = false;
            }
        } else if (!sampling_started) {
            display_gnss_pps(valid_pps_count, calibrated, current_ppm);
        }

        if (sampling_started && (flags & GNSS_1PPS_UPDATED) &&
            states->gnss_time_locked) {
            display_gnss_locked(states->gnss_satellites,
                                states->gnss_hdop, current_ppm);
        }
    }
}

void task_send_packet(void* argument) {
    scl3300_global_states_t* states = (scl3300_global_states_t*)argument;
    scl3300_acquisition_message_t acq_msg;
    uint32_t expected_sequence = 0;
    bool has_previous_sequence = false;
    uint8_t message_idx = 0;
    int64_t message_timestamp = 0;
    bool packet_gnss_enabled = false;
    packet_gnss_location_t packet_location = {0};

    for (;;) {
        if (osMessageQueueGet(states->sensor_acquisition_queue, &acq_msg,
                              NULL, osWaitForever) != osOK) {
            continue;
        }
        if (has_previous_sequence && acq_msg.sequence != expected_sequence) {
            message_idx = 0;
        }
        expected_sequence = acq_msg.sequence + 1;
        has_previous_sequence = true;

        if (message_idx == 0) {
            message_timestamp = acq_msg.timestamp;
            packet_gnss_enabled = false;
            packet_location.valid = false;
            if (states->use_gnss_time) {
                taskENTER_CRITICAL();
                if (states->gnss_time_locked) {
                    packet_location = states->gnss_location;
                    packet_gnss_enabled = true;
                }
                taskEXIT_CRITICAL();
            }
        }

        for (uint8_t channel = 0; channel < 3; channel++) {
            states->accel_acquisition_channel_buffer[channel][message_idx] =
                acq_msg.accel_data[channel];
        }

        if (++message_idx == states->channel_chunk_length) {
            if (!states->use_gnss_time || packet_gnss_enabled) {
                send_data_packet(states, message_timestamp, packet_gnss_enabled,
                                 &packet_location);
            }
            message_idx = 0;
        }
    }
}

void task_sensor_acquire(void* argument) {
    scl3300_global_states_t* states = (scl3300_global_states_t*)argument;
    scl3300_acquisition_message_t acq_msg;
    uint32_t sample_sequence = 0;
    uint32_t period_ms = 1000 / states->sample_rate;

    peri_sensor_init();
    osThreadFlagsWait(SENSOR_ACQUIRE_ACT, osFlagsWaitAny, osWaitForever);
    uint32_t next_sample_time = (uint32_t)mcu_utils_systime_get_uptime_ms();

    for (;;) {
        uint32_t now = (uint32_t)mcu_utils_systime_get_uptime_ms();
        if ((int32_t)(now - next_sample_time) < 0) {
            osDelay(1);
            continue;
        }
        next_sample_time += period_ms;
        if ((int32_t)(now - next_sample_time) >= 0) {
            next_sample_time = now + period_ms;
        }

        acq_msg.timestamp = mcu_utils_systime_get_unix_time_ms();
        get_accel_readout(acq_msg.accel_data);
        acq_msg.sequence = sample_sequence++;
        if (osMessageQueuePut(states->sensor_acquisition_queue, &acq_msg,
                              0, 0) != osOK) {
            states->sensor_queue_overflow_count++;
        }
    }
}

void system_setup(void) {
    mcu_utils_systime_init();
    MX_DMA_Init();
    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    static scl3300_global_states_t states;
    read_device_settings(&states);
    irq_states = &states;
    if (states.use_gnss_time) {
        gnss_pps_init(&states.gnss_pps_state);
    }
    mcu_utils_i2c_init(false);
    mcu_utils_spi_init(false);
    mcu_utils_uart_init(states.baud_rate, false);
    peri_screen_init();
    display_device_settings(&states);

    peri_gnss_init(states.use_gnss_time);

    const static osMessageQueueAttr_t sensor_acquisition_queue_attr = {
        .name = "sensor_acquisition_data",
        .cb_mem = &sensor_acquisition_queue_cb,
        .cb_size = sizeof(sensor_acquisition_queue_cb),
        .mq_mem = sensor_acquisition_queue_mem,
        .mq_size = sizeof(sensor_acquisition_queue_mem),
    };
    states.sensor_acquisition_queue = osMessageQueueNew(
        SENSOR_ACQUISITION_QUEUE_DEPTH, sizeof(scl3300_acquisition_message_t),
        &sensor_acquisition_queue_attr);
    if (states.sensor_acquisition_queue == NULL) {
        Error_Handler();
    }

    const static osThreadAttr_t send_packet_task_attr = {
        .name = "send_packet",
        .cb_mem = &send_packet_task_cb,
        .cb_size = sizeof(send_packet_task_cb),
        .stack_mem = send_packet_task_stack,
        .stack_size = sizeof(send_packet_task_stack),
        .priority = osPriorityNormal,
    };
    if (osThreadNew(task_send_packet, &states, &send_packet_task_attr) == NULL) {
        Error_Handler();
    }

    const static osThreadAttr_t sensor_acquire_task_attr = {
        .name = "sensor_acquire",
        .cb_mem = &sensor_acquire_task_cb,
        .cb_size = sizeof(sensor_acquire_task_cb),
        .stack_mem = sensor_acquire_task_stack,
        .stack_size = sizeof(sensor_acquire_task_stack),
        .priority = osPriorityAboveNormal,
    };
    states.sensor_acquire_task_handle =
        osThreadNew(task_sensor_acquire, &states, &sensor_acquire_task_attr);
    if (states.sensor_acquire_task_handle == NULL) {
        Error_Handler();
    }

    if (states.use_gnss_time) {
        const static osThreadAttr_t gnss_acquire_task_attr = {
            .name = "gnss_acquire",
            .cb_mem = &gnss_acquire_task_cb,
            .cb_size = sizeof(gnss_acquire_task_cb),
            .stack_mem = gnss_acquire_task_stack,
            .stack_size = sizeof(gnss_acquire_task_stack),
            .priority = osPriorityNormal,
        };
        states.gnss_acquire_task_handle =
            osThreadNew(task_gnss_acquire, &states, &gnss_acquire_task_attr);
        if (states.gnss_acquire_task_handle == NULL) {
            Error_Handler();
        }
    } else {
        osThreadFlagsSet(states.sensor_acquire_task_handle, SENSOR_ACQUIRE_ACT);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    osKernelInitialize();
    system_setup();
    osKernelStart();

    while (1) {
        ;
    }
}
