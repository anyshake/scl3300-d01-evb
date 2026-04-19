#include "User/Inc/scl3300/utils.h"

void scl3300_init(scl3300_ctl_pin_t ctl_pin, bool is_rtos) {
    mcu_utils_gpio_mode(ctl_pin.cs, MCU_UTILS_GPIO_MODE_OUTPUT);
    mcu_utils_gpio_high(ctl_pin.cs);
    mcu_utils_delay_ms(1000, is_rtos);
}

static uint8_t scl3300_get_checksum(uint32_t data) {
    uint8_t crc = 0xFF;

    for (uint8_t bit = 31; bit > 7; bit--) {
        uint8_t msb = crc & 0x80;
        crc <<= 1;

        if ((data >> bit) & 1) {
            msb ^= 0x80;
        }
        if (msb) {
            crc ^= 0x1D;
        }
    }

    return ~crc;
}

static void scl3300_nop(void) {
    uint64_t current_time = mcu_utils_uptime_get_us();
    while (mcu_utils_uptime_get_us() - current_time < 10) {
        ;
    }
}

void scl3300_set_reg_bank(scl3300_ctl_pin_t ctl_pin, uint8_t bank_id) {
    scl3300_write_reg(ctl_pin, SCL3300_REG_BANK_SEL, bank_id);
}

void scl3300_read_reg(scl3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t* rx_data) {
    uint32_t request = ((reg & 0x1F) << 26) |   // register address
                       ((0x00 & 0x03) << 24) |  // return status as dummy byte
                       ((0x00 & 0x0F) << 8);    // data area always 0 for read operation
    request |= scl3300_get_checksum(request);   // CRC-8 checksum
    uint8_t* request_bytes = (uint8_t*)&request;

    mcu_utils_gpio_low(ctl_pin.cs);
    for (uint8_t i = sizeof(request); i > 0; i--) {
        mcu_utils_spi_transfer(request_bytes[i - 1]);
    }
    mcu_utils_gpio_high(ctl_pin.cs);

    scl3300_nop();  // required delay for read operation

    // Send frame again for gettings response data
    mcu_utils_gpio_low(ctl_pin.cs);
    mcu_utils_spi_transfer(request_bytes[3]);
    uint8_t high_byte = mcu_utils_spi_transfer(request_bytes[2]);
    uint8_t low_byte = mcu_utils_spi_transfer(request_bytes[1]);
    mcu_utils_spi_transfer(request_bytes[0]);
    mcu_utils_gpio_high(ctl_pin.cs);

    *rx_data = ((uint16_t)high_byte << 8) | low_byte;
}

void scl3300_write_reg(scl3300_ctl_pin_t ctl_pin, uint8_t reg, uint16_t tx_data) {
    uint32_t request = ((reg | 0x20) << 26) |             // register address
                       ((0x00 & 0x03) << 24) |            // return status as dummy byte
                       (((tx_data >> 8) & 0xFF) << 16) |  // high byte data
                       ((tx_data & 0xFF) << 8);           // low byte data
    request |= scl3300_get_checksum(request);             // CRC-8 checksum
    uint8_t* request_bytes = (uint8_t*)&request;

    mcu_utils_gpio_low(ctl_pin.cs);
    for (uint8_t i = sizeof(request); i > 0; i--) {
        mcu_utils_spi_transfer(request_bytes[i - 1]);
    }
    mcu_utils_gpio_high(ctl_pin.cs);
}
