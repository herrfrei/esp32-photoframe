#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void rgb888_to_rgb565(const uint8_t *src_rgb888, uint16_t w, uint16_t h, uint8_t *dst_rgb565);

#ifdef __cplusplus
}
#endif
