#ifndef __SCA3300_REGS_ERR_FLAG2_H__
#define __SCA3300_REGS_ERR_FLAG2_H__

#include <stdint.h>
#include "User/Inc/sca3300/utils.h"

typedef struct {
    // Description: External capacitor connection error (digital domain)
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t d_ext_c;
    // Description: External capacitor connection error (analog domain)
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t a_ext_c;
    // Description: AGND connection error
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t agnd;
    // Description: Supply voltage error
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t vdd;
    // Description: Operation mode changed by user
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Operation mode changed by user
    uint8_t mode_change;
    // Description: Device in power down mode
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Device in power down mode
    uint8_t pd;
    // Description: Memory CRC check failed
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Memory CRC check failed
    uint8_t memory_crc;
    // Description: Analog power error.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t apwr;
    // Description: Digital power indication.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1:
    //   * After start-up or reset: This flag is set high. No actions needed
    //   * During operation: Digital power error. Component failure possible. SW or HW reset needed
    uint8_t dpwr;
    // Description: Reference voltage error.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t vref;
    // Description: Analog power error
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t apwr_2;
    // Description: Temperature signal path saturated.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: External temperature too high or low. Component failure possible
    uint8_t temp_sat;
    // Description: Clock error.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected
    uint8_t clk;
} sca3300_reg_err_flag2_t;

sca3300_reg_err_flag2_t sca3300_reg_new_err_flag2(void);
void sca3300_reg_get_err_flag2(sca3300_ctl_pin_t ctl_pin, sca3300_reg_err_flag2_t* err_flag2);

#endif
