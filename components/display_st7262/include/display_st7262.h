#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void display_st7262_init(void);
void display_st7262_set_backlight(bool on);
void display_st7262_flush_rgb565(const uint8_t *buf, uint16_t w, uint16_t h);

#ifdef __cplusplus
}
#endif
