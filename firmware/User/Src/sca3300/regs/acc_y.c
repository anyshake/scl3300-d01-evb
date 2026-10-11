#include "User/Inc/sca3300/regs/acc_y.h"

sca3300_reg_acc_y_t sca3300_reg_new_acc_y(void) {
    sca3300_reg_acc_y_t acc_y;
    acc_y.acc_y = 0xFFFF;
    return acc_y;
}

void sca3300_reg_get_acc_y(sca3300_ctl_pin_t ctl_pin, sca3300_reg_acc_y_t* acc_y) {
    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_ACC_Y, (uint16_t*)&acc_y->acc_y);
}
