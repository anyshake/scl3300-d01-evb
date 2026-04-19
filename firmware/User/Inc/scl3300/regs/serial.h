#ifndef __SCL3300_REGS_SERIAL_H__
#define __SCL3300_REGS_SERIAL_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Component serial part 1
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t serial1;
    // Description: Component serial part 2
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t serial2;
} scl3300_reg_serial_t;

scl3300_reg_serial_t scl3300_reg_new_serial(void);
void scl3300_reg_get_serial(scl3300_ctl_pin_t ctl_pin, scl3300_reg_serial_t* serial);

#endif
