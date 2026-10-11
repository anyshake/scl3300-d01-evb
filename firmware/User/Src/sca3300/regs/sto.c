#include "User/Inc/sca3300/regs/sto.h"

sca3300_reg_sto_t sca3300_reg_new_sto(void) {
    sca3300_reg_sto_t sto;
    sto.sto = 0xFFFF;
    return sto;
}

void sca3300_reg_get_sto(sca3300_ctl_pin_t ctl_pin, sca3300_reg_sto_t* sto) {
    sca3300_set_reg_bank(ctl_pin, 0);
    sca3300_read_reg(ctl_pin, SCA3300_REG_BANK0_STO, (uint16_t*)&sto->sto);
}
