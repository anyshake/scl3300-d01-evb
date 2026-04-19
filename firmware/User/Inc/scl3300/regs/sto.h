#ifndef __SCL3300_REGS_STO_H__
#define __SCL3300_REGS_STO_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Self-test output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    int16_t sto;
} scl3300_reg_sto_t;

scl3300_reg_sto_t scl3300_reg_new_sto(void);
void scl3300_reg_get_sto(scl3300_ctl_pin_t ctl_pin, scl3300_reg_sto_t* sto);

#endif
