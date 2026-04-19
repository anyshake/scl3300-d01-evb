#ifndef __SCL3300_REGS_ACC_Y_H__
#define __SCL3300_REGS_ACC_Y_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Y-axis acceleration output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    int16_t acc_y;
} scl3300_reg_acc_y_t;

scl3300_reg_acc_y_t scl3300_reg_new_acc_y(void);
void scl3300_reg_get_acc_y(scl3300_ctl_pin_t ctl_pin, scl3300_reg_acc_y_t* acc_y);

#endif
