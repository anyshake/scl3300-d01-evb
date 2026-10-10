#ifndef __PERIPHERAL_H
#define __PERIPHERAL_H

#include <stdbool.h>

void peri_screen_init(void);
void peri_gnss_init(bool enabled);
void peri_sensor_init(void);

#endif
