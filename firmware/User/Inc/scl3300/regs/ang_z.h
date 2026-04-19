#ifndef __SCL3300_REGS_ANG_Z_H__
#define __SCL3300_REGS_ANG_Z_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Z-axis angle output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    uint16_t ang_z;
} scl3300_reg_ang_z_t;

scl3300_reg_ang_z_t scl3300_reg_new_ang_z(void);
void scl3300_reg_get_ang_z(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_z_t* ang_z);

#endif
