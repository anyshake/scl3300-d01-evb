#include "User/Inc/scl3300/regs/whoami.h"

scl3300_reg_whoami_t scl3300_reg_new_whoami(void) {
    scl3300_reg_whoami_t whoami;
    whoami.component_id = 0xC1;
    return whoami;
}

void scl3300_reg_get_whoami(scl3300_ctl_pin_t ctl_pin, scl3300_reg_whoami_t* whoami) {
    uint16_t whoami_data;

    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_WHOAMI, &whoami_data);

    whoami->component_id = whoami_data & 0xFF;
}
