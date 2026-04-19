#ifndef __SCL3300_UTILS_H__
#define __SCL3300_UTILS_H__

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/spi.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/scl3300/regs_map.h"

typedef struct {
    mcu_utils_gpio_t cs;
} scl3300_ctl_pin_t;

void scl3300_init(scl3300_ctl_pin_t ctl_pin, bool is_rtos);
void scl3300_set_reg_bank(scl3300_ctl_pin_t ctl_pin, uint8_t bank_id);
void scl3300_read_reg(scl3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t* rx_data);
void scl3300_write_reg(scl3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t tx_data);

#endif
