#ifndef __SCL3300_REGS_WHOAMI_H__
#define __SCL3300_REGS_WHOAMI_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: For component identification, the value is 0xC1.
    // Permissions: RO
    // Default: 8'hC1
    uint8_t component_id;
} scl3300_reg_whoami_t;

scl3300_reg_whoami_t scl3300_reg_new_whoami(void);
void scl3300_reg_get_whoami(scl3300_ctl_pin_t ctl_pin, scl3300_reg_whoami_t* whoami);

#endif
