#include "User/Inc/sca3300/regs/acc_z.h"

sca3300_reg_acc_z_t sca3300_reg_new_acc_z(void) {
    sca3300_reg_acc_z_t acc_z;
    acc_z.acc_z = 0xFFFF;
    return acc_z;
}

void sca3300_reg_get_acc_z(sca3300_ctl_pin_t ctl_pin, sca3300_reg_acc_z_t* acc_z) {
    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_ACC_Z, (uint16_t*)&acc_z->acc_z);
}
