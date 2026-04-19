#ifndef __SCL3300_REGS_STATUS_H__
#define __SCL3300_REGS_STATUS_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Digital block error type 1.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected, SW or HW reset needed
    uint8_t dig1;
    // Description: Digital block error type 2.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Error detected, SW or HW reset needed
    uint8_t dig2;
    // Description: Clock error.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Clock error detected, SW or HW reset needed
    uint8_t clk;
    // Description: Signal saturated in signal path.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Acceleration too high and acceleration and inclination readings not usable. Component failure possible. All acceleration, inclination, and STO output data is invalid.
    uint8_t sat;
    // Description: Temperature signal path saturated.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: External temperature too high or low. Component failure possible
    uint8_t temp_sat;
    // Description: Start-up indication or Voltage level failure.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1:
    //   * After start-up or reset: This flag is set high. No actions needed
    //   * During operation: External voltages too high or low. Component failure possible. SW or HW reset needed
    uint8_t pwr;
    // Description: Error in non-volatile memory.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Memory check failed. Possible component failure. SW or HW reset needed
    uint8_t mem;
    // Description: Device in power down mode.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Device in power down mode. If power down is not requested, SW or HW reset needed
    uint8_t pd;
    // Description: Operation mode changed.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Operation mode changed. If mode change is not requested, SW or HW reset needed
    uint8_t mode_change;
    // Description: Component internal connection error.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Component internal connection error. Possible component failure.
    uint8_t pin_continuity;
} scl3300_reg_status_t;

scl3300_reg_status_t scl3300_reg_new_status(void);
void scl3300_reg_get_status(scl3300_ctl_pin_t ctl_pin, scl3300_reg_status_t* status);

#endif
