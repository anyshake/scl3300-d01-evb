#ifndef __SCL3300_REGS_ANG_X_H__
#define __SCL3300_REGS_ANG_X_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: X-axis angle output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t ang_x;
} scl3300_reg_ang_x_t;

scl3300_reg_ang_x_t scl3300_reg_new_ang_x(void);
void scl3300_reg_get_ang_x(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_x_t* ang_x);

#endif
