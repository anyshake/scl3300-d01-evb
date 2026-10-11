#ifndef __SCA3300_UTILS_H__
#define __SCA3300_UTILS_H__

#include <stdbool.h>
#include <stdint.h>

#include "Utils/Inc/delay.h"
#include "Utils/Inc/gpio.h"
#include "Utils/Inc/spi.h"
#include "Utils/Inc/uptime.h"

#include "User/Inc/sca3300/regs_map.h"

typedef struct {
    mcu_utils_gpio_t cs;
} sca3300_ctl_pin_t;

void sca3300_init(sca3300_ctl_pin_t ctl_pin, bool is_rtos);
void sca3300_set_reg_bank(sca3300_ctl_pin_t ctl_pin, uint8_t bank_id);
void sca3300_read_reg(sca3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t* rx_data);
void sca3300_write_reg(sca3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t tx_data);

#endif
