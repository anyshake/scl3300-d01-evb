#ifndef __SCL3300_REGS_ANG_Y_H__
#define __SCL3300_REGS_ANG_Y_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Y-axis angle output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t ang_y;
} scl3300_reg_ang_y_t;

scl3300_reg_ang_y_t scl3300_reg_new_ang_y(void);
void scl3300_reg_get_ang_y(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_y_t* ang_y);

#endif
