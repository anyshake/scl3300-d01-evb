#include "User/Inc/scl3300/regs/sto.h"

scl3300_reg_sto_t scl3300_reg_new_sto(void) {
    scl3300_reg_sto_t sto;
    sto.sto = 0xFFFF;
    return sto;
}

void scl3300_reg_get_sto(scl3300_ctl_pin_t ctl_pin, scl3300_reg_sto_t* sto) {
    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_STO, (uint16_t*)&sto->sto);
}
