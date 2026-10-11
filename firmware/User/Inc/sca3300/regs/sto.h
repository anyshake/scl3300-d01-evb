#ifndef __SCA3300_REGS_STO_H__
#define __SCA3300_REGS_STO_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: Self-test output in 2’s complement format.
    // Permissions: RO
    // Default: 16'hFFFF
    int16_t sto;
} sca3300_reg_sto_t;

sca3300_reg_sto_t sca3300_reg_new_sto(void);
void sca3300_reg_get_sto(sca3300_ctl_pin_t ctl_pin, sca3300_reg_sto_t* sto);

#endif
