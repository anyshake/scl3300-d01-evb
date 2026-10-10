#ifndef __USER_UTILS_H
#define __USER_UTILS_H

#include "User/Inc/types.h"

void read_device_settings(scl3300_global_states_t* states);
void display_device_settings(const scl3300_global_states_t* states);
void display_gnss_pps(uint8_t valid_pps_count, bool calibrated, float ppm);
void display_gnss_acquiring(uint8_t satellites, float hdop, float ppm);
void display_gnss_locked(uint8_t satellites, float hdop, float ppm);
void display_gnss_lost(void);
bool parse_gnss_message(uint8_t* message_buf, gnss_status_t* gnss_status,
                        gnss_location_t* gnss_location, gnss_time_t* gnss_time,
                        int64_t local_timestamp_us, int64_t* time_offset_us,
                        bool is_rtos);

#endif
