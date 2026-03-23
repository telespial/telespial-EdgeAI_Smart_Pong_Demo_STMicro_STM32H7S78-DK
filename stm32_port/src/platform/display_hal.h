#pragma once

#include <stdbool.h>
#include <stdint.h>

bool display_hal_init(void);
void display_hal_fill(uint16_t rgb565);
void display_hal_blit_rect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t *rgb565);

/* Optional helper for board-specific flush hooks. */
const uint16_t *display_hal_framebuffer(void);
uint32_t display_hal_width(void);
uint32_t display_hal_height(void);
