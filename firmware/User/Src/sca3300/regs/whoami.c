#include "User/Inc/sca3300/regs/whoami.h"

sca3300_reg_whoami_t sca3300_reg_new_whoami(void) {
    sca3300_reg_whoami_t whoami;
    whoami.component_id = 0x51;
    return whoami;
}

void sca3300_reg_get_whoami(sca3300_ctl_pin_t ctl_pin, sca3300_reg_whoami_t* whoami) {
    uint16_t whoami_data;

    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_WHOAMI, &whoami_data);

    whoami->component_id = whoami_data & 0xFF;
}
