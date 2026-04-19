#ifndef __SCL3300_REGS_TEMPERATURE_H__
#define __SCL3300_REGS_TEMPERATURE_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Temperature output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t temperature;
} scl3300_reg_temperature_t;

scl3300_reg_temperature_t scl3300_reg_new_temperature(void);
void scl3300_reg_get_temperature(scl3300_ctl_pin_t ctl_pin, scl3300_reg_temperature_t* temperature);

#endif
