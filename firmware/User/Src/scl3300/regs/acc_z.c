#include "User/Inc/scl3300/regs/acc_z.h"

scl3300_reg_acc_z_t scl3300_reg_new_acc_z(void) {
    scl3300_reg_acc_z_t acc_z;
    acc_z.acc_z = 0xFFFF;
    return acc_z;
}

void scl3300_reg_get_acc_z(scl3300_ctl_pin_t ctl_pin, scl3300_reg_acc_z_t* acc_z) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ACC_Z, (uint16_t*)&acc_z->acc_z);
}
