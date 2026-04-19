#ifndef __SCL3300_REGS_ANG_CTRL_H__
#define __SCL3300_REGS_ANG_CTRL_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

#define __SCL3300_REG_ANG_CTRL_DEFAULT_VALUE 0x0000

typedef struct {
    // Description: Enable angle outputs.
    // Permissions: R/W
    // Default: 5'h00
    // Options:
    // - 5'h00: Disable angle outputs
    // - 5'h1F: Enable angle outputs
    uint8_t enable_angle;
} scl3300_reg_ang_ctrl_t;

scl3300_reg_ang_ctrl_t scl3300_reg_new_ang_ctrl(void);
void scl3300_reg_set_ang_ctrl(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_ctrl_t* ang_ctrl);
void scl3300_reg_get_ang_ctrl(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_ctrl_t* ang_ctrl);

#endif
