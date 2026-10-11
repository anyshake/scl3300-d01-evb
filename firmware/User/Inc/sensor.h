#ifndef __SENSOR_H
#define __SENSOR_H

typedef enum {
    SENSOR_TYPE_UNKNOWN = 0,
    SENSOR_TYPE_SCL3300,
    SENSOR_TYPE_SCA3300,
} sensor_type_t;

sensor_type_t sensor_get_type(void);
void sensor_set_type(sensor_type_t sensor_type);

#endif
