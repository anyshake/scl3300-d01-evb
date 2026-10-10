#include "User/Inc/peripheral.h"

#include "User/Inc/gnss/model.h"
#include "User/Inc/settings.h"
#include "User/Inc/scl3300/regs_entry.h"
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

    scl3300_reg_whoami_t whoami = scl3300_reg_new_whoami();
    do {
        scl3300_reg_get_whoami(SCL3300_CS_PIN, &whoami);
        if (whoami.component_id != 0xC1) {
            osDelay(1000);
        }
    } while (whoami.component_id != 0xC1);

    scl3300_reg_cmd_t cmd = scl3300_reg_new_cmd();
    cmd.mode = 0b11;
    scl3300_reg_set_cmd(SCL3300_CS_PIN, &cmd);
    osDelay(100);
}
