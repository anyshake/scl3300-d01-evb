#include "User/Inc/sca3300/regs/serial.h"

sca3300_reg_serial_t sca3300_reg_new_serial(void) {
    sca3300_reg_serial_t serial;
    serial.serial1 = 0xFFFF;
    serial.serial2 = 0xFFFF;
    return serial;
}

void sca3300_reg_get_serial(sca3300_ctl_pin_t ctl_pin, sca3300_reg_serial_t* serial) {
    sca3300_set_reg_bank(ctl_pin, 1);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK1_SERIAL1, &serial->serial1);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK1_SERIAL2, &serial->serial2);
}
