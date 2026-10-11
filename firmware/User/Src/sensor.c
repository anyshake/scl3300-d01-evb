#include "User/Inc/sensor.h"

static volatile sensor_type_t current_sensor_type = SENSOR_TYPE_UNKNOWN;

sensor_type_t sensor_get_type(void) {
    return current_sensor_type;
}

void sensor_set_type(sensor_type_t sensor_type) {
    current_sensor_type = sensor_type;
}
