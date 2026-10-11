#include "User/Inc/sca3300/regs/cmd.h"

sca3300_reg_cmd_t sca3300_reg_new_cmd(void) {
    sca3300_reg_cmd_t cmd;
    cmd.sw_rst = 0;
    cmd.pd = 0;
    cmd.mode = 0;
    return cmd;
}

void sca3300_reg_set_cmd(sca3300_ctl_pin_t ctl_pin, sca3300_reg_cmd_t* cmd) {
    uint16_t cmd_data = __SCA3300_REG_CMD_DEFAULT_VALUE;

    cmd_data &= ~(0x01 << 5);
    cmd_data |= (cmd->sw_rst & 0x01) << 5;
    cmd_data &= ~(0x01 << 2);
    cmd_data |= (cmd->pd & 0x01) << 2;
    cmd_data &= ~(0x03 << 0);
    cmd_data |= (cmd->mode & 0x03) << 0;

    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_write_reg(ctl_pin, SCA3300_REG_BANK0_MODE, cmd_data);
}

void sca3300_reg_get_cmd(sca3300_ctl_pin_t ctl_pin, sca3300_reg_cmd_t* cmd) {
    uint16_t cmd_data;

    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_MODE, &cmd_data);

    cmd->sw_rst = (cmd_data >> 5) & 0x01;
    cmd->pd = (cmd_data >> 2) & 0x01;
    cmd->mode = cmd_data & 0x03;
}
