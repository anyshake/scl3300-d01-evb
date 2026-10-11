#include "User/Inc/sca3300/regs/err_flag1.h"

sca3300_reg_err_flag1_t sca3300_reg_new_err_flag1(void) {
    sca3300_reg_err_flag1_t err_flag1;
    err_flag1.adc_sat = 0;
    err_flag1.afe_sat = 0;
    err_flag1.mem = 0;
    return err_flag1;
}

void sca3300_reg_get_err_flag1(sca3300_ctl_pin_t ctl_pin, sca3300_reg_err_flag1_t* err_flag1) {
    uint16_t err_flag1_data = 0;

    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_ERR_FLAG1, &err_flag1_data);

    err_flag1->adc_sat = (err_flag1_data >> 11) & 0x01;
    err_flag1->afe_sat = (err_flag1_data >> 1) & 0x3FF;
    err_flag1->mem = err_flag1_data & 0x01;
}
