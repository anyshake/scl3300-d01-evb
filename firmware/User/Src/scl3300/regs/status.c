#include "User/Inc/scl3300/regs/status.h"

scl3300_reg_status_t scl3300_reg_new_status(void) {
    scl3300_reg_status_t status;
    status.dig1 = 0;
    status.dig2 = 0;
    status.clk = 0;
    status.sat = 0;
    status.temp_sat = 0;
    status.pwr = 0;
    status.mem = 0;
    status.pd = 0;
    status.mode_change = 0;
    status.pin_continuity = 0;
    return status;
}

void scl3300_reg_get_status(scl3300_ctl_pin_t ctl_pin, scl3300_reg_status_t* status) {
    uint16_t status_data;

    scl3300_set_reg_bank(ctl_pin, 0);
    scl3300_read_reg(ctl_pin, SCL3300_REG_BANK0_STATUS, &status_data);

    status->dig1 = (status_data >> 9) & 0x01;
    status->dig2 = (status_data >> 8) & 0x01;
    status->clk = (status_data >> 7) & 0x01;
    status->sat = (status_data >> 6) & 0x01;
    status->temp_sat = (status_data >> 5) & 0x01;
    status->pwr = (status_data >> 4) & 0x01;
    status->mem = (status_data >> 3) & 0x01;
    status->pd = (status_data >> 2) & 0x01;
    status->mode_change = (status_data >> 1) & 0x01;
    status->pin_continuity = status_data & 0x01;
}
