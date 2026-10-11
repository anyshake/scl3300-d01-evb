#include "User/Inc/sca3300/regs/acc_x.h"

sca3300_reg_acc_x_t sca3300_reg_new_acc_x(void) {
    sca3300_reg_acc_x_t acc_x;
    acc_x.acc_x = 0xFFFF;
    return acc_x;
}

void sca3300_reg_get_acc_x(sca3300_ctl_pin_t ctl_pin, sca3300_reg_acc_x_t* acc_x) {
    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_ACC_X, (uint16_t*)&acc_x->acc_x);
}
