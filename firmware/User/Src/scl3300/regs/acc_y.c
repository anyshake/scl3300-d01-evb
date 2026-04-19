#include "User/Inc/scl3300/regs/acc_y.h"

scl3300_reg_acc_y_t scl3300_reg_new_acc_y(void) {
    scl3300_reg_acc_y_t acc_y;
    acc_y.acc_y = 0xFFFF;
    return acc_y;
}

void scl3300_reg_get_acc_y(scl3300_ctl_pin_t ctl_pin, scl3300_reg_acc_y_t* acc_y) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ACC_Y, (uint16_t*)&acc_y->acc_y);
}
