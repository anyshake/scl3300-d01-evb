#ifndef __SCA3300_REGS_ACC_Y_H__
#define __SCA3300_REGS_ACC_Y_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: Y-axis acceleration output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    int16_t acc_y;
} sca3300_reg_acc_y_t;

sca3300_reg_acc_y_t sca3300_reg_new_acc_y(void);
void sca3300_reg_get_acc_y(sca3300_ctl_pin_t ctl_pin, sca3300_reg_acc_y_t* acc_y);

#endif
