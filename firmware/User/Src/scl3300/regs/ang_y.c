#include "User/Inc/scl3300/regs/ang_y.h"

scl3300_reg_ang_y_t scl3300_reg_new_ang_y(void) {
    scl3300_reg_ang_y_t ang_y;
    ang_y.ang_y = 0xFFFF;
    return ang_y;
}

void scl3300_reg_get_ang_y(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_y_t* ang_y) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ANG_Y, &ang_y->ang_y);
}
