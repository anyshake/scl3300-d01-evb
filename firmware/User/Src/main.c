#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Core/Inc/dma.h"
#include "Core/Inc/main.h"
#include "Core/Inc/tim.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/i2c.h"
#include "Utils/Inc/led.h"
#include "Utils/Inc/spi.h"
#include "Utils/Inc/uart.h"
#include "Utils/Inc/uart2.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/gnss/utils.h"

#include "User/Inc/scl3300/regs_entry.h"
#include "User/Inc/scl3300/utils.h"

#define SAMPLE_RATE 100
#define CHANNEL_SAMPLES 5

static const mcu_utils_gpio_t MCU_STATE_PIN = {
    .port = GPIOC,
    .pin = GPIO_PIN_13,
};
static const gnss_ctl_pin_t GNSS_CTL_PIN = {
    .pps = {.port = GPIOA, .pin = GPIO_PIN_8},
    .rst = {.port = GPIOA, .pin = GPIO_PIN_15},
};
static const scl3300_ctl_pin_t SCL3300_CS_PIN = {
    .cs = {.port = GPIOA, .pin = GPIO_PIN_4},
};

static uint8_t data_acquisition_task_stack[4096];
static StaticTask_t data_acquisition_task_cb;
static osThreadId_t data_acquisition_task_handle;

void task_data_acquisition(void* argument) {
    scl3300_init(SCL3300_CS_PIN, true);

    scl3300_reg_cmd_t cmd = scl3300_reg_new_cmd();
    cmd.mode = 0b11;
    scl3300_reg_set_cmd(SCL3300_CS_PIN, &cmd);

    static int16_t x_buf[CHANNEL_SAMPLES];
    static int16_t y_buf[CHANNEL_SAMPLES];
    static int16_t z_buf[CHANNEL_SAMPLES];

    static uint8_t sample_idx = 0;
    static uint32_t next_sample_time = 0;

    uint8_t msg[1024];

    for (;;) {
        uint32_t now = mcu_utils_uptime_get_ms();
        if (now < next_sample_time) {
            continue;
        }
        next_sample_time = now + (1000 / SAMPLE_RATE);

        scl3300_reg_acc_x_t acc_x = scl3300_reg_new_acc_x();
        scl3300_reg_get_acc_x(SCL3300_CS_PIN, &acc_x);
        x_buf[sample_idx] = acc_x.acc_x;

        scl3300_reg_acc_y_t acc_y = scl3300_reg_new_acc_y();
        scl3300_reg_get_acc_y(SCL3300_CS_PIN, &acc_y);
        y_buf[sample_idx] = acc_y.acc_y;

        scl3300_reg_acc_z_t acc_z = scl3300_reg_new_acc_z();
        scl3300_reg_get_acc_z(SCL3300_CS_PIN, &acc_z);
        z_buf[sample_idx] = acc_z.acc_z;

        sample_idx++;

        if (sample_idx == CHANNEL_SAMPLES) {
            uint16_t offset = 0;

            for (uint8_t i = 0; i < CHANNEL_SAMPLES; i++) {
                offset += snprintf((char*)&msg[offset],
                                   sizeof(msg) - offset,
                                   "%d,", x_buf[i]);
            }

            for (uint8_t i = 0; i < CHANNEL_SAMPLES; i++) {
                offset += snprintf((char*)&msg[offset],
                                   sizeof(msg) - offset,
                                   "%d,", y_buf[i]);
            }

            for (uint8_t i = 0; i < CHANNEL_SAMPLES; i++) {
                offset += snprintf((char*)&msg[offset],
                                   sizeof(msg) - offset,
                                   "%d,", z_buf[i]);
            }

            offset += snprintf((char*)&msg[offset],
                               sizeof(msg) - offset,
                               "%d\r\n", SAMPLE_RATE);
            mcu_utils_uart_write(msg, offset, false);

            sample_idx = 0;
        }
    }
}

void system_setup(void) {
    MX_DMA_Init();
    MX_TIM3_Init();
    HAL_TIM_Base_Start_IT(&htim3);

    mcu_utils_gpio_init(false);
    mcu_utils_led_blink(MCU_STATE_PIN, 3, false);
    mcu_utils_gpio_high(MCU_STATE_PIN);

    mcu_utils_i2c_init(false);
    mcu_utils_spi_init(false);
    mcu_utils_uart_init(115200, false);
    mcu_utils_uart2_init(GNSS_BAUDRATE, false);

    gnss_init(GNSS_CTL_PIN);
    gnss_reset(GNSS_CTL_PIN, false);
    gnss_model_setup(false);

    const static osThreadAttr_t data_acquisition_task_attr = {
        .name = "data_acquisition",
        .cb_mem = &data_acquisition_task_cb,
        .cb_size = sizeof(data_acquisition_task_cb),
        .stack_mem = data_acquisition_task_stack,
        .stack_size = sizeof(data_acquisition_task_stack),
        .priority = osPriorityNormal,
    };
    data_acquisition_task_handle = osThreadNew(task_data_acquisition, NULL, &data_acquisition_task_attr);
    if (data_acquisition_task_handle == NULL) {
        Error_Handler();
    }
}

int main(void) {
    SystemClock_Config();
    HAL_Init();

    osKernelInitialize();
    system_setup();
    osKernelStart();

    while (1) {
        ;
    }
}
