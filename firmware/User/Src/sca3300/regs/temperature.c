#include "User/Inc/sca3300/regs/temperature.h"

sca3300_reg_temperature_t sca3300_reg_new_temperature(void) {
    sca3300_reg_temperature_t temperature;
    temperature.temperature = 0xFFFF;
    return temperature;
}

void sca3300_reg_get_temperature(sca3300_ctl_pin_t ctl_pin, sca3300_reg_temperature_t* temperature) {
    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_TEMPERATURE, &temperature->temperature);
}
