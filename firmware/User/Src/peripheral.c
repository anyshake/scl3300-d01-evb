#include "User/Inc/peripheral.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/sca3300/regs_entry.h"
#include "User/Inc/scl3300/regs_entry.h"
#include "User/Inc/sensor.h"
#include "User/Inc/settings.h"
#include "User/Inc/ssd1306/utils.h"
#include "Utils/Inc/uart2.h"

void peri_screen_init(void) {
    ssd1306_init();
    ssd1306_enable();
    ssd1306_clear();
}

void peri_gnss_init(bool enabled) {
    gnss_init(GNSS_CTL_PIN);
    if (enabled) {
        mcu_utils_uart2_init(GNSS_BAUDRATE, false);
        gnss_reset(GNSS_CTL_PIN, false);
        gnss_model_setup(false);
    } else {
        mcu_utils_gpio_low(GNSS_CTL_PIN.rst);
    }
}

void peri_sensor_init(void) {
    scl3300_init(SCL3300_CS_PIN, true);

    sensor_set_type(SENSOR_TYPE_UNKNOWN);
    while (sensor_get_type() == SENSOR_TYPE_UNKNOWN) {
        scl3300_reg_whoami_t scl_whoami = scl3300_reg_new_whoami();
        scl3300_reg_get_whoami(SCL3300_CS_PIN, &scl_whoami);
        if (scl_whoami.component_id == 0xC1) {
            sensor_set_type(SENSOR_TYPE_SCL3300);
            break;
        }

        sca3300_reg_whoami_t sca_whoami = sca3300_reg_new_whoami();
        sca3300_reg_get_whoami(SCA3300_CS_PIN, &sca_whoami);
        if (sca_whoami.component_id == 0x51) {
            sensor_set_type(SENSOR_TYPE_SCA3300);
            break;
        }

        osDelay(1000);
    }

    if (sensor_get_type() == SENSOR_TYPE_SCA3300) {
        sca3300_reg_cmd_t cmd = sca3300_reg_new_cmd();
        cmd.mode = 0b11;
        sca3300_reg_set_cmd(SCA3300_CS_PIN, &cmd);
    } else {
        scl3300_reg_cmd_t cmd = scl3300_reg_new_cmd();
        cmd.mode = 0b11;
        scl3300_reg_set_cmd(SCL3300_CS_PIN, &cmd);
    }
    osDelay(100);
}
