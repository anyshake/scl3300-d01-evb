#include "User/Inc/reader.h"
#include "User/Inc/sca3300/regs_entry.h"
#include "User/Inc/scl3300/regs_entry.h"
#include "User/Inc/sensor.h"
#include "User/Inc/settings.h"

static void get_scl3300_accel_readout(int16_t arr[3]) {
    scl3300_reg_acc_x_t x = scl3300_reg_new_acc_x();
    scl3300_reg_acc_y_t y = scl3300_reg_new_acc_y();
    scl3300_reg_acc_z_t z = scl3300_reg_new_acc_z();

    scl3300_reg_get_acc_x(SCL3300_CS_PIN, &x);
    scl3300_reg_get_acc_y(SCL3300_CS_PIN, &y);
    scl3300_reg_get_acc_z(SCL3300_CS_PIN, &z);

    arr[0] = z.acc_z;
    arr[1] = x.acc_x;
    arr[2] = y.acc_y;
}

static void get_sca3300_accel_readout(int16_t arr[3]) {
    sca3300_reg_acc_x_t x = sca3300_reg_new_acc_x();
    sca3300_reg_acc_y_t y = sca3300_reg_new_acc_y();
    sca3300_reg_acc_z_t z = sca3300_reg_new_acc_z();

    sca3300_reg_get_acc_x(SCA3300_CS_PIN, &x);
    sca3300_reg_get_acc_y(SCA3300_CS_PIN, &y);
    sca3300_reg_get_acc_z(SCA3300_CS_PIN, &z);

    arr[0] = z.acc_z;
    arr[1] = x.acc_x;
    arr[2] = y.acc_y;
}

void get_accel_readout(int16_t arr[3]) {
    if (sensor_get_type() == SENSOR_TYPE_SCA3300) {
        get_sca3300_accel_readout(arr);
    } else {
        get_scl3300_accel_readout(arr);
    }
}
