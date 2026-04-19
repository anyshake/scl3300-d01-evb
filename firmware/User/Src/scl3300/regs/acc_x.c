#include "User/Inc/scl3300/regs/acc_x.h"

scl3300_reg_acc_x_t scl3300_reg_new_acc_x(void) {
    scl3300_reg_acc_x_t acc_x;
    acc_x.acc_x = 0xFFFF;
    return acc_x;
}

void scl3300_reg_get_acc_x(scl3300_ctl_pin_t ctl_pin, scl3300_reg_acc_x_t* acc_x) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ACC_X, (uint16_t*)&acc_x->acc_x);
}
