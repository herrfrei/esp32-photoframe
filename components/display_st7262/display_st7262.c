#include "display_st7262.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define ST7262_WIDTH  800
#define ST7262_HEIGHT 480

#define ST7262_BACKLIGHT_PIN GPIO_NUM_38
#define ST7262_RESET_PIN     GPIO_NUM_21

static const char *TAG = "display_st7262";
static esp_lcd_panel_handle_t g_panel = NULL;
static bool g_initialized = false;

static void st7262_reset(void)
{
    gpio_set_level(ST7262_RESET_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(ST7262_RESET_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(20));
}

static void st7262_gpio_init(void)
{
    gpio_config_t conf = {
        .pin_bit_mask = (1ULL << ST7262_BACKLIGHT_PIN) |
                        (1ULL << ST7262_RESET_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&conf));

    gpio_set_level(ST7262_BACKLIGHT_PIN, 0);
    gpio_set_level(ST7262_RESET_PIN, 1);
}

static esp_err_t st7262_panel_init(void)
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

void display_st7262_init(void)
{
    if (g_initialized) {
        return;
    }

    st7262_gpio_init();
    st7262_reset();
    ESP_ERROR_CHECK(st7262_panel_init());

    display_st7262_set_backlight(true);
    g_initialized = true;
}

void display_st7262_set_backlight(bool on)
{
    gpio_set_level(ST7262_BACKLIGHT_PIN, on ? 1 : 0);
}

void display_st7262_flush_rgb565(const uint8_t *buf, uint16_t w, uint16_t h)
{
    if (!g_initialized || !buf || !g_panel) {
        return;
    }

    if (w != ST7262_WIDTH || h != ST7262_HEIGHT) {
        ESP_LOGW(TAG, "Unexpected frame size: %u x %u", w, h);
    }

    esp_lcd_panel_draw_bitmap(g_panel, 0, 0, w, h, (const void *)buf);
}
