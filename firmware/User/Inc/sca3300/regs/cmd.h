#ifndef __SCA3300_REGS_CMD_H__
#define __SCA3300_REGS_CMD_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

#define __SCA3300_REG_CMD_DEFAULT_VALUE 0x0000

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
    // - 2'b00: Mode 1, 3g full-scale, 70 Hz 1st order low-pass filter
    // - 2'b01: Mode 2, 6g full-scale, 70 Hz 1st order low-pass filter
    // - 2'b10: Mode 3, 1.5g full-scale, 70 Hz 1st order low-pass filter
    // - 2'b11: Mode 4, 1.5g full-scale, 10 Hz 1st order low-pass filter, low noise mode
    uint8_t mode;
} sca3300_reg_cmd_t;

sca3300_reg_cmd_t sca3300_reg_new_cmd(void);
void sca3300_reg_set_cmd(sca3300_ctl_pin_t ctl_pin, sca3300_reg_cmd_t* cmd);
void sca3300_reg_get_cmd(sca3300_ctl_pin_t ctl_pin, sca3300_reg_cmd_t* cmd);

#endif
