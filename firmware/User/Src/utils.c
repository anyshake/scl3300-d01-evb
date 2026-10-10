#include <stdio.h>
#include <string.h>

#include "User/Inc/utils.h"
#include "User/Inc/gnss/model.h"
#include "User/Inc/gnss/parse.h"
#include "User/Inc/gnss/time.h"
#include "User/Inc/ssd1306/display.h"
#include "User/Inc/ssd1306/font.h"
#include "Utils/Inc/systime.h"

void read_device_settings(scl3300_global_states_t* states) {
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT_PULLDOWN);
    mcu_utils_gpio_mode(SAMPLERATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT_PULLDOWN);
    switch ((mcu_utils_gpio_read(SAMPLERATE_SELECT_P1) << 1) |
            mcu_utils_gpio_read(SAMPLERATE_SELECT_P2)) {
        case 0: states->sample_rate = 250; break;
        case 1: states->sample_rate = 200; break;
        case 2: states->sample_rate = 100; break;
        case 3: states->sample_rate = 50; break;
    }

    mcu_utils_gpio_mode(BAUDRATE_SELECT_P1, MCU_UTILS_GPIO_MODE_INPUT_PULLDOWN);
    mcu_utils_gpio_mode(BAUDRATE_SELECT_P2, MCU_UTILS_GPIO_MODE_INPUT_PULLDOWN);
    switch ((mcu_utils_gpio_read(BAUDRATE_SELECT_P1) << 1) |
            mcu_utils_gpio_read(BAUDRATE_SELECT_P2)) {
        case 0: states->baud_rate = 57600; break;
        case 1: states->baud_rate = 115200; break;
        case 2: states->baud_rate = 230400; break;
        case 3: states->baud_rate = 460800; break;
    }

    mcu_utils_gpio_mode(GNSS_SELECT_PIN, MCU_UTILS_GPIO_MODE_INPUT_PULLDOWN);
    states->use_gnss_time = mcu_utils_gpio_read(GNSS_SELECT_PIN) != 0;
    states->packet_sending_interval = PACKET_SENDING_INTERVAL;
    states->channel_chunk_length = states->sample_rate * PACKET_SENDING_INTERVAL / 1000;
}

void display_device_settings(const scl3300_global_states_t* states) {
    char line[22];

    ssd1306_display_string(0, 0, "SCL3300-D01", SSD1306_FONT_TYPE_ASCII_8X16,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf(line, sizeof(line), "SAMPLE RATE: %3u Hz", (unsigned)states->sample_rate);
    ssd1306_display_string(0, 2, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf(line, sizeof(line), "BAUD RATE: %lu", (unsigned long)states->baud_rate);
    ssd1306_display_string(0, 3, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
    snprintf(line, sizeof(line), "GNSS MODE: %s", states->use_gnss_time ? "ON" : "OFF");
    ssd1306_display_string(0, 4, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, false);
}

static void display_gnss_metrics(uint8_t satellites, float hdop) {
    char line[32];
    unsigned hdop_tenths = (unsigned)(hdop * 10.0f + 0.5f);

    snprintf(line, sizeof(line), "SAT: %2u HDOP: %u.%u",
             (unsigned)satellites, hdop_tenths / 10, hdop_tenths % 10);
    ssd1306_display_string(0, 6, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
}

static void display_gnss_ppm(float ppm) {
    char line[32];
    int32_t ppm_hundredths = (int32_t)(ppm * 100.0f + (ppm >= 0 ? 0.5f : -0.5f));
    char sign = ppm_hundredths < 0 ? '-' : '+';
    uint32_t magnitude = ppm_hundredths < 0
                             ? (uint32_t)(-ppm_hundredths)
                             : (uint32_t)ppm_hundredths;

    snprintf(line, sizeof(line), "Clock Dev: %c%lu.%02lu ppm", sign,
             (unsigned long)(magnitude / 100),
             (unsigned long)(magnitude % 100));
    ssd1306_display_string(0, 7, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
}

void display_gnss_pps(uint8_t valid_pps_count, bool calibrated, float ppm) {
    char line[22];
    if (valid_pps_count == 0) {
        snprintf(line, sizeof(line), "GNSS: WAIT 1PPS");
    } else if (valid_pps_count < GNSS_PPS_VALID_VERIFY) {
        snprintf(line, sizeof(line), "PPS LOCK: %2u/%u", valid_pps_count,
                 GNSS_PPS_VALID_VERIFY);
    } else if (!calibrated) {
        snprintf(line, sizeof(line), "GNSS: CALIBRATING");
    } else {
        snprintf(line, sizeof(line), "GNSS: VERIFYING TIME");
    }
    ssd1306_display_string(0, 5, line, SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    if (!calibrated) {
        ssd1306_display_string(0, 6, "SAT: -- HDOP: --",
                               SSD1306_FONT_TYPE_ASCII_8X6,
                               SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
        ssd1306_display_string(0, 7, "Clock Dev: -- ppm",
                               SSD1306_FONT_TYPE_ASCII_8X6,
                               SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    } else {
        display_gnss_ppm(ppm);
    }
}

void display_gnss_acquiring(uint8_t satellites, float hdop, float ppm) {
    ssd1306_display_string(0, 5, "GNSS: VERIFYING TIME",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    display_gnss_metrics(satellites, hdop);
    display_gnss_ppm(ppm);
}

void display_gnss_locked(uint8_t satellites, float hdop, float ppm) {
    ssd1306_display_string(0, 5, "GNSS: TIME LOCKED",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    display_gnss_metrics(satellites, hdop);
    display_gnss_ppm(ppm);
}

void display_gnss_lost(void) {
    ssd1306_display_string(0, 5, "GNSS: TIME LOST",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    ssd1306_display_string(0, 6, "Waiting for RMC/GGA",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
    ssd1306_display_string(0, 7, "Clock Dev: -- ppm",
                           SSD1306_FONT_TYPE_ASCII_8X6,
                           SSD1306_FONT_DISPLAY_COLOR_WHITE, true);
}

bool parse_gnss_message(uint8_t* message_buf, gnss_status_t* gnss_status,
                        gnss_location_t* gnss_location, gnss_time_t* gnss_time,
                        int64_t local_timestamp_us, int64_t* time_offset_us,
                        bool is_rtos) {
    bool got_gga = false;
    bool got_rmc = false;
    gnss_status_t parsed_status = {.hdop = 999.0f};
    gnss_time_t rmc_time = {0};
    gnss_time_t gga_time = {0};
    gnss_location_t rmc_location = {0};
    gnss_location_t gga_location = {0};

    while (!got_gga || !got_rmc) {
        if (!gnss_get_sentence(message_buf, 990, is_rtos)) {
            return false;
        }
        if (gnss_padding_sentence(message_buf) == 0) {
            continue;
        }

        if (!got_gga && gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_GGA)) {
            gnss_parse_gga(message_buf, &parsed_status, &gga_location, &gga_time);
            got_gga = gga_time.is_valid;
        } else if (!got_rmc &&
                   gnss_match_keyword(message_buf, GNSS_SENTENCE_TYPE_RMC)) {
            gnss_parse_rmc(message_buf, &rmc_location, &rmc_time);
            got_rmc = rmc_time.is_valid;
        }

        if (got_gga && got_rmc &&
            gga_time.hour == rmc_time.hour &&
            gga_time.minute == rmc_time.minute &&
            gga_time.second == rmc_time.second &&
            gga_time.milisecond == rmc_time.milisecond) {
            if (gnss_status != NULL) {
                *gnss_status = parsed_status;
            }
            if (gnss_location != NULL) {
                *gnss_location = rmc_location;
                gnss_location->elevation = gga_location.elevation;
                gnss_location->is_valid = rmc_location.is_valid &&
                                          gga_location.is_valid;
            }
            if (gnss_time != NULL && time_offset_us != NULL) {
                int64_t epoch_ms = gnss_model_handle_timestamp(
                    gnss_get_timestamp(&rmc_time));
                *time_offset_us = epoch_ms * 1000 - local_timestamp_us;
                *gnss_time = rmc_time;
            }
            return true;
        }

        if (got_gga && got_rmc) {
            got_gga = false;
            got_rmc = false;
            memset(&rmc_time, 0, sizeof(rmc_time));
            memset(&gga_time, 0, sizeof(gga_time));
            memset(&rmc_location, 0, sizeof(rmc_location));
            memset(&gga_location, 0, sizeof(gga_location));
        }
    }

    return false;
}
