#ifndef __SCL3300_REGS_CMD_H__
#define __SCL3300_REGS_CMD_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

#define __SCL3300_REG_CMD_DEFAULT_VALUE 0x0000

typedef struct {
    // Description: Software reset. Writing 1 to this bit performs a software reset. This bit is self-clearing.
    // Permissions: R/W
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Trigger software reset
    uint8_t sw_rst;
    // Description: Power-down mode. Writing 1 to this bit puts the device into power-down mode.
    // Permissions: R/W
    // Default: 1'b0
    // Options:
    // - 1'b0: Power-up mode
    // - 1'b1: Power-down mode
    uint8_t pd;
    // Description: Measurement mode, available modes are defined in the options below. Writing to this bit changes the measurement mode immediately.
    // Permissions: R/W
    // Default: 2'b0
    // Options:
    // - 2'b00: Mode 1, 1.2g full-scale, 40 Hz 1st order low-pass filter
    // - 2'b01: Mode 2, 2.4g full-scale, 70 Hz 1st order low-pass filter
    // - 2'b10: Mode 3, inclination mode, 10 Hz 1st order low-pass filter
    // - 2'b11: Mode 4, inclination mode, 10 Hz 1st order low-pass filter, low noise mode
    uint8_t mode;
} scl3300_reg_cmd_t;

scl3300_reg_cmd_t scl3300_reg_new_cmd(void);
void scl3300_reg_set_cmd(scl3300_ctl_pin_t ctl_pin, scl3300_reg_cmd_t* cmd);
void scl3300_reg_get_cmd(scl3300_ctl_pin_t ctl_pin, scl3300_reg_cmd_t* cmd);

#endif
