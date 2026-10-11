#ifndef __SCA3300_REGS_TEMPERATURE_H__
#define __SCA3300_REGS_TEMPERATURE_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: Temperature output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t temperature;
} sca3300_reg_temperature_t;

sca3300_reg_temperature_t sca3300_reg_new_temperature(void);
void sca3300_reg_get_temperature(sca3300_ctl_pin_t ctl_pin, sca3300_reg_temperature_t* temperature);

#endif
