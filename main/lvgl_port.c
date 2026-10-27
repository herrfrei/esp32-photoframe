#include "lvgl.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "display_st7262.h"

#define LVGL_WIDTH  800
#define LVGL_HEIGHT 480
#define LVGL_BUF_SIZE (LVGL_WIDTH * 20)

static const char *TAG = "lvgl_port";
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;

static void disp_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t w = area->x2 - area->x1 + 1;
    uint16_t h = area->y2 - area->y1 + 1;

    uint16_t *rgb565 = (uint16_t *)heap_caps_malloc(w * h * sizeof(uint16_t), MALLOC_CAP_SPIRAM);
    if (!rgb565) {
        lv_disp_flush_ready(drv);
        return;
    }

    for (int y = 0; y < h; y++) {
        const lv_color_t *src = color_p + (y * w);
        for (int x = 0; x < w; x++) {
            rgb565[y * w + x] = src[x].full;
        }
    }

    display_st7262_flush_rgb565((const uint8_t *)rgb565, w, h);
    heap_caps_free(rgb565);

    lv_disp_flush_ready(drv);
}

void lvgl_port_init(void)
{
    lv_init();

    static lv_color_t *buf1 = NULL;
    static lv_color_t *buf2 = NULL;

    buf1 = heap_caps_malloc(LVGL_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    buf2 = heap_caps_malloc(LVGL_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);

    if (!buf1 || !buf2) {
        ESP_LOGE(TAG, "Failed to allocate LVGL buffers");
        return;
    }

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LVGL_BUF_SIZE);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LVGL_WIDTH;
    disp_drv.ver_res = LVGL_HEIGHT;
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "LVGL initialized");
}
