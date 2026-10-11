#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "User/Inc/gnss/utils.h"
#include "User/Inc/sca3300/utils.h"
#include "User/Inc/scl3300/utils.h"
#include "Utils/Inc/gpio.h"

#define PACKET_SENDING_INTERVAL 100
#define CHANNEL_SAMPLE_CAPACITY 25
#define SENSOR_ACQUISITION_QUEUE_DEPTH 8
#define GNSS_REQUIRED_VALID_HDOP 2.0f
#define GNSS_RESYNC_INTERVAL_MS (10 * 60 * 1000)

static const mcu_utils_gpio_t SAMPLERATE_SELECT_P1 = {.port = GPIOB, .pin = GPIO_PIN_3};
static const mcu_utils_gpio_t SAMPLERATE_SELECT_P2 = {.port = GPIOB, .pin = GPIO_PIN_4};
static const mcu_utils_gpio_t BAUDRATE_SELECT_P1 = {.port = GPIOB, .pin = GPIO_PIN_11};
static const mcu_utils_gpio_t BAUDRATE_SELECT_P2 = {.port = GPIOB, .pin = GPIO_PIN_10};
static const mcu_utils_gpio_t GNSS_SELECT_PIN = {.port = GPIOB, .pin = GPIO_PIN_12};
static const mcu_utils_gpio_t MCU_STATE_PIN = {.port = GPIOC, .pin = GPIO_PIN_13};
static const gnss_ctl_pin_t GNSS_CTL_PIN = {
    .pps = {.port = GPIOA, .pin = GPIO_PIN_8},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_15},
};
static const scl3300_ctl_pin_t SCL3300_CS_PIN = {
    .cs = {.port = GPIOA, .pin = GPIO_PIN_4},
};
static const sca3300_ctl_pin_t SCA3300_CS_PIN = {
    .cs = {.port = GPIOA, .pin = GPIO_PIN_4},
};

#endif
