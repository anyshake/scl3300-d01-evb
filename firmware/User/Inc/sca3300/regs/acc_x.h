#ifndef __SCA3300_REGS_ACC_X_H__
#define __SCA3300_REGS_ACC_X_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: X-axis acceleration output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    int16_t acc_x;
} sca3300_reg_acc_x_t;

sca3300_reg_acc_x_t sca3300_reg_new_acc_x(void);
void sca3300_reg_get_acc_x(sca3300_ctl_pin_t ctl_pin, sca3300_reg_acc_x_t* acc_x);

#endif
