#ifndef __SCL3300_REGS_ERR_FLAG1_H__
#define __SCL3300_REGS_ERR_FLAG1_H__

#include <stdint.h>
#include "User/Inc/scl3300/utils.h"

typedef struct {
    // Description: Signal saturated at Analog to-Digital Converter (ADC).
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Signal saturated at ADC
    uint8_t adc_sat;
    // Description: Signal saturated at C2V
    // Permissions: RO
    // Default: 10'h000
    uint8_t afe_sat;
    // Description: Error in non-volatile memory.
    // Permissions: RO
    // Default: 1'b0
    // Options:
    // - 1'b0: Normal operation
    // - 1'b1: Memory error detected
    uint8_t mem;
} scl3300_reg_err_flag1_t;

scl3300_reg_err_flag1_t scl3300_reg_new_err_flag1(void);
void scl3300_reg_get_err_flag1(scl3300_ctl_pin_t ctl_pin, scl3300_reg_err_flag1_t* err_flag1);

#endif
