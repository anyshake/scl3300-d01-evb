#ifndef __SCA3300_REGS_WHOAMI_H__
#define __SCA3300_REGS_WHOAMI_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: For component identification, the value is 0x51.
    // Permissions: RO
    // Default: 8'h51
    uint8_t component_id;
} sca3300_reg_whoami_t;

sca3300_reg_whoami_t sca3300_reg_new_whoami(void);
void sca3300_reg_get_whoami(sca3300_ctl_pin_t ctl_pin, sca3300_reg_whoami_t* whoami);

#endif
