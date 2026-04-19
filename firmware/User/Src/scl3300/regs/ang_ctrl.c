#include "User/Inc/scl3300/regs/ang_ctrl.h"

scl3300_reg_ang_ctrl_t scl3300_reg_new_ang_ctrl(void) {
    scl3300_reg_ang_ctrl_t ang_ctrl;
    ang_ctrl.enable_angle = 0x00;
    return ang_ctrl;
}

void scl3300_reg_set_ang_ctrl(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_ctrl_t* ang_ctrl) {
    uint16_t ang_ctrl_data = __SCL3300_REG_ANG_CTRL_DEFAULT_VALUE;

    ang_ctrl_data &= ~(0x1F << 0);
    ang_ctrl_data |= (ang_ctrl->enable_angle & 0x1F) << 0;

    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_write_reg(ctl_pin, SCL3300_REG_BANK0_ANG_CTRL, ang_ctrl_data);
}

void scl3300_reg_get_ang_ctrl(scl3300_ctl_pin_t ctl_pin, scl3300_reg_ang_ctrl_t* ang_ctrl) {
    uint16_t ang_ctrl_data;

    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_ANG_CTRL, &ang_ctrl_data);

    ang_ctrl->enable_angle = ang_ctrl_data & 0x1F;
}
