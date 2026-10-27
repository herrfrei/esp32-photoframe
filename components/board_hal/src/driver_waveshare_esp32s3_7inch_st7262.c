#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "board_hal.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

static const char *TAG = "board_hal_waveshare_st7262";

#define ST7262_WIDTH 800
#define ST7262_HEIGHT 480

static esp_lcd_panel_handle_t g_panel = NULL;
static bool g_initialized = false;

static void st7262_backlight(bool on)
{
    gpio_set_level(BOARD_HAL_BACKLIGHT_PIN, on ? 1 : 0);
}

static void st7262_reset(void)
{
    if (BOARD_HAL_RESET_PIN >= 0) {
        gpio_set_level(BOARD_HAL_RESET_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(20));
        gpio_set_level(BOARD_HAL_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

static esp_err_t st7262_init_panel(void)
{
    esp_lcd_rgb_panel_config_t panel_cfg = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .timings = {
            .pclk_hz = 16000000,
            .h_res = ST7262_WIDTH,
            .v_res = ST7262_HEIGHT,
            .hsync_back_porch = 10,
            .hsync_front_porch = 10,
            .hsync_pulse_width = 8,
            .vsync_back_porch = 10,
            .vsync_front_porch = 10,
            .vsync_pulse_width = 8,
            .pclk_idle_low = false,
            .pclk_active_neg = true,
            .de_idle_high = false,
            .vsync_idle_low = false,
            .hsync_idle_low = false,
        },
        .data_width = 16,
        .num_fbs = 2,
        .bounce_buffer_size_px = 800 * 20,
        .sram_trans_align = 8,
        .psram_trans_align = 64,
        .flags = {
            .fb_in_psram = 1,
            .double_buffer = 1,
            .refresh_on_demand = 0,
        },
    };

    esp_err_t ret = esp_lcd_new_rgb_panel(&panel_cfg, &g_panel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_new_rgb_panel failed: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_init(g_panel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "esp_lcd_panel_init failed: %s", esp_err_to_name(ret));
        return ret;
    }

    return ESP_OK;
}

static void st7262_gpio_init(void)
{
    gpio_config_t gpio_conf = {
        .pin_bit_mask = (1ULL << BOARD_HAL_BACKLIGHT_PIN) |
                        (1ULL << BOARD_HAL_RESET_PIN) |
                        (1ULL << BOARD_HAL_LED_RED_PIN) |
                        (1ULL << BOARD_HAL_LED_GREEN_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&gpio_conf));

    gpio_set_level(BOARD_HAL_BACKLIGHT_PIN, 0);
    gpio_set_level(BOARD_HAL_RESET_PIN, 1);
    gpio_set_level(BOARD_HAL_LED_RED_PIN, 0);
    gpio_set_level(BOARD_HAL_LED_GREEN_PIN, 0);
}

esp_err_t board_hal_init(void)
{
    if (g_initialized) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Initializing Waveshare ESP32-S3 7-inch ST7262 board HAL");

    st7262_gpio_init();
    st7262_reset();
    ESP_ERROR_CHECK(st7262_init_panel());
    st7262_backlight(true);

    g_initialized = true;
    return ESP_OK;
}

esp_err_t board_hal_prepare_for_sleep(void)
{
    ESP_LOGI(TAG, "Preparing ST7262 board for sleep");
    st7262_backlight(false);
    return ESP_OK;
}

bool board_hal_is_battery_connected(void)
{
    return false;
}

int board_hal_get_battery_percent(void)
{
    return -1;
}

int board_hal_get_battery_voltage(void)
{
    return -1;
}

bool board_hal_is_charging(void)
{
    return false;
}

bool board_hal_is_usb_connected(void)
{
    return false;
}

void board_hal_shutdown(void)
{
    st7262_backlight(false);
}

esp_err_t board_hal_get_temperature(float *t)
{
    if (!t) {
        return ESP_ERR_INVALID_ARG;
    }
    *t = 0.0f;
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t board_hal_get_humidity(float *h)
{
    if (!h) {
        return ESP_ERR_INVALID_ARG;
    }
    *h = 0.0f;
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t board_hal_rtc_init(void)
{
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t board_hal_rtc_get_time(time_t *t)
{
    if (!t) {
        return ESP_ERR_INVALID_ARG;
    }
    *t = 0;
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t board_hal_rtc_set_time(time_t t)
{
    (void)t;
    return ESP_ERR_NOT_SUPPORTED;
}

bool board_hal_rtc_is_available(void)
{
    return false;
}

void board_hal_led_set(board_hal_led_t led, bool on)
{
    switch (led) {
    case BOARD_HAL_LED_POWER:
        gpio_set_level(BOARD_HAL_LED_RED_PIN, on ? 1 : 0);
        break;
    case BOARD_HAL_LED_ACTIVITY:
        gpio_set_level(BOARD_HAL_LED_GREEN_PIN, on ? 1 : 0);
        break;
    default:
        break;
    }
}
