#include "User/Inc/scl3300/regs/ang_x.h"

scl3300_reg_ang_x_t scl3300_reg_new_ang_x(void) {
    scl3300_reg_ang_x_t ang_x;
    ang_x.ang_x = 0xFFFF;
    return ang_x;
}

void scl3300_reg_get_ang_x(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_x_t* ang_x) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ANG_X, &ang_x->ang_x);
}
