#include "rgb565.h"

void rgb888_to_rgb565(const uint8_t *src_rgb888, uint16_t w, uint16_t h, uint8_t *dst_rgb565)
{
    uint16_t *dst = (uint16_t *)dst_rgb565;
    int idx = 0;

    for (uint16_t y = 0; y < h; y++) {
        for (uint16_t x = 0; x < w; x++) {
            const uint8_t *p = src_rgb888 + (idx * 3);
            uint8_t r = p[0];
            uint8_t g = p[1];
            uint8_t b = p[2];

            uint16_t rr = (r >> 3) & 0x1F;
            uint16_t gg = (g >> 2) & 0x3F;
            uint16_t bb = (b >> 3) & 0x1F;

            dst[y * w + x] = (uint16_t)((rr << 11) | (gg << 5) | bb);
            idx++;
        }
    }
}
