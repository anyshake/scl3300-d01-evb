#include "User/Inc/scl3300/regs/ang_z.h"

scl3300_reg_ang_z_t scl3300_reg_new_ang_z(void) {
    scl3300_reg_ang_z_t ang_z;
    ang_z.ang_z = 0xFFFF;
    return ang_z;
}

void scl3300_reg_get_ang_z(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_z_t* ang_z) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ANG_Z, &ang_z->ang_z);
}
