#ifndef __SCA3300_REGS_SERIAL_H__
#define __SCA3300_REGS_SERIAL_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: Component serial part 1
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t serial1;
    // Description: Component serial part 2
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t serial2;
} sca3300_reg_serial_t;

sca3300_reg_serial_t sca3300_reg_new_serial(void);
void sca3300_reg_get_serial(sca3300_ctl_pin_t ctl_pin, sca3300_reg_serial_t* serial);

#endif
