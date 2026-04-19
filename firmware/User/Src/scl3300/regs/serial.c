#include "User/Inc/scl3300/regs/serial.h"

scl3300_reg_serial_t scl3300_reg_new_serial(void) {
    scl3300_reg_serial_t serial;
    serial.serial1 = 0xFFFF;
    serial.serial2 = 0xFFFF;
    return serial;
}

void scl3300_reg_get_serial(scl3300_ctl_pin_t ctl_pin, scl3300_reg_serial_t* serial) {
    scl3300_set_reg_bank(ctl_pin, 1);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK1_SERIAL1, &serial->serial1);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK1_SERIAL2, &serial->serial2);
}
