#pragma once

#include <hal/gpio_types.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "driver/gpio.h"
#include "epaper.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BOARD_HAL_DISPLAY_WIDTH epaper_get_width()
#define BOARD_HAL_DISPLAY_HEIGHT epaper_get_height()

typedef enum {
    BOARD_TYPE_WAVESHARE_PHOTOPAINTER,
    BOARD_TYPE_WAVESHARE_ESP32S3_7INCH_ST7262,
    BOARD_TYPE_SEEEDSTUDIO_XIAO_EE02,
    BOARD_TYPE_SEEEDSTUDIO_XIAO_EE03,
    BOARD_TYPE_SEEEDSTUDIO_XIAO_EE04,
    BOARD_TYPE_SEEEDSTUDIO_RETERMINAL_E1002,
    BOARD_TYPE_SEEEDSTUDIO_RETERMINAL_E1003,
    BOARD_TYPE_SEEEDSTUDIO_RETERMINAL_E1004,
    BOARD_TYPE_M5STACK_M5PAPER_V11,
    BOARD_TYPE_UNKNOWN
} board_type_t;

#ifdef CONFIG_BOARD_DRIVER_WAVESHARE_PHOTOPAINTER_73
#include "board_waveshare_photopainter_73.h"
#elif defined(CONFIG_BOARD_DRIVER_WAVESHARE_ESP32S3_7INCH_ST7262)
#include "board_waveshare_esp32s3_7inch_st7262.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_XIAO_EE02)
#include "board_seeedstudio_xiao_ee02.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_XIAO_EE03)
#include "board_seeedstudio_xiao_ee03.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_XIAO_EE04)
#include "board_seeedstudio_xiao_ee04.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_RETERMINAL_E1002)
#include "board_seeedstudio_reterminal_e1002.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_RETERMINAL_E1003)
#include "board_seeedstudio_reterminal_e1003.h"
#elif defined(CONFIG_BOARD_DRIVER_SEEEDSTUDIO_RETERMINAL_E1004)
#include "board_seeedstudio_reterminal_e1004.h"
#elif defined(CONFIG_BOARD_DRIVER_M5STACK_M5PAPER_V11)
#include "board_m5stack_m5paper_v11.h"
#else
#error "No board selected! Please define CONFIG_BOARD_DRIVER_..."
#endif

#ifndef BOARD_HAL_DISPLAY_TYPE
#define BOARD_HAL_DISPLAY_TYPE "st7262_rgb"
#endif

esp_err_t board_hal_init(void);
esp_err_t board_hal_prepare_for_sleep(void);
bool board_hal_is_battery_connected(void);
int board_hal_get_battery_percent(void);
int board_hal_get_battery_voltage(void);
bool board_hal_is_charging(void);
bool board_hal_is_usb_connected(void);
void board_hal_shutdown(void);
esp_err_t board_hal_get_temperature(float *t);
esp_err_t board_hal_get_humidity(float *h);
esp_err_t board_hal_rtc_init(void);
esp_err_t board_hal_rtc_get_time(time_t *t);
esp_err_t board_hal_rtc_set_time(time_t t);
bool board_hal_rtc_is_available(void);

typedef enum {
    BOARD_HAL_LED_POWER,
    BOARD_HAL_LED_ACTIVITY,
} board_hal_led_t;

void board_hal_led_set(board_hal_led_t led, bool on);

#ifdef __cplusplus
}
#endif
