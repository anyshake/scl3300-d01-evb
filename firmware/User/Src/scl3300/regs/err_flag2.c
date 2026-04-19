#include "User/Inc/scl3300/regs/err_flag2.h"

scl3300_reg_err_flag2_t scl3300_reg_new_err_flag2(void) {
    scl3300_reg_err_flag2_t err_flag2;
    err_flag2.d_ext_c = 0;
    err_flag2.a_ext_c = 0;
    err_flag2.agnd = 0;
    err_flag2.vdd = 0;
    err_flag2.mode_change = 0;
    err_flag2.pd = 0;
    err_flag2.memory_crc = 0;
    err_flag2.apwr = 0;
    err_flag2.dpwr = 0;
    err_flag2.vref = 0;
    err_flag2.apwr_2 = 0;
    err_flag2.temp_sat = 0;
    err_flag2.clk = 0;
    return err_flag2;
}

void scl3300_reg_get_err_flag2(scl3300_ctl_pin_t ctl_pin, scl3300_reg_err_flag2_t* err_flag2) {
    uint16_t err_flag2_data;

    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_STATUS, &err_flag2_data);

    err_flag2->d_ext_c = (err_flag2_data >> 14) & 0x01;
    err_flag2->a_ext_c = (err_flag2_data >> 13) & 0x01;
    err_flag2->agnd = (err_flag2_data >> 12) & 0x01;
    err_flag2->vdd = (err_flag2_data >> 11) & 0x01;
    err_flag2->mode_change = (err_flag2_data >> 9) & 0x01;
    err_flag2->pd = (err_flag2_data >> 8) & 0x01;
    err_flag2->memory_crc = (err_flag2_data >> 7) & 0x01;
    err_flag2->apwr = (err_flag2_data >> 5) & 0x01;
    err_flag2->dpwr = (err_flag2_data >> 4) & 0x01;
    err_flag2->vref = (err_flag2_data >> 3) & 0x01;
    err_flag2->apwr_2 = (err_flag2_data >> 2) & 0x01;
    err_flag2->temp_sat = (err_flag2_data >> 1) & 0x01;
    err_flag2->clk = err_flag2_data & 0x01;
}
