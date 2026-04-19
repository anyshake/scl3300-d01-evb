#include "User/Inc/scl3300/regs/temperature.h"

scl3300_reg_temperature_t scl3300_reg_new_temperature(void) {
    scl3300_reg_temperature_t temperature;
    temperature.temperature = 0xFFFF;
    return temperature;
}

void scl3300_reg_get_temperature(scl3300_ctl_pin_t ctl_pin, scl3300_reg_temperature_t* temperature) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_TEMPERATURE, &temperature->temperature);
}
