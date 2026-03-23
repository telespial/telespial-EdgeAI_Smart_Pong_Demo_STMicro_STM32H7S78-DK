#include "platform/display_hal.h"

#include <string.h>

#include "edgeai_config.h"

#ifndef EDGEAI_FRAMEBUFFER_BASE
#define EDGEAI_FRAMEBUFFER_BASE 0x90000000u
#endif

static uint16_t *s_fb = (uint16_t *)EDGEAI_FRAMEBUFFER_BASE;

__attribute__((weak)) void stm32_display_present_rgb565(const uint16_t *fb, uint32_t w, uint32_t h)
{
    (void)fb;
    (void)w;
    (void)h;
}

bool display_hal_init(void)
{
    memset(s_fb, 0, (size_t)(EDGEAI_LCD_W * EDGEAI_LCD_H * (int32_t)sizeof(uint16_t)));
    stm32_display_present_rgb565(s_fb, EDGEAI_LCD_W, EDGEAI_LCD_H);
    return true;
}

void display_hal_fill(uint16_t rgb565)
{
    for (uint32_t i = 0; i < (uint32_t)(EDGEAI_LCD_W * EDGEAI_LCD_H); i++)
    {
        s_fb[i] = rgb565;
    }
    stm32_display_present_rgb565(s_fb, EDGEAI_LCD_W, EDGEAI_LCD_H);
}

void display_hal_blit_rect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t *rgb565)
{
    if (!rgb565) return;
    if (x1 < x0 || y1 < y0) return;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= EDGEAI_LCD_W) x1 = EDGEAI_LCD_W - 1;
    if (y1 >= EDGEAI_LCD_H) y1 = EDGEAI_LCD_H - 1;
    if (x1 < x0 || y1 < y0) return;

    uint32_t w = (uint32_t)((x1 - x0) + 1);
    uint32_t h = (uint32_t)((y1 - y0) + 1);

    for (uint32_t r = 0; r < h; r++)
    {
        uint16_t *dst = &s_fb[(uint32_t)(y0 + (int32_t)r) * (uint32_t)EDGEAI_LCD_W + (uint32_t)x0];
        const uint16_t *src = &rgb565[r * w];
        memcpy(dst, src, w * sizeof(uint16_t));
    }

    /* Early bring-up behavior: push after each blit for visible progress.
     * Replace with dirty-rect or vblank-based flush in board integration.
     */
    stm32_display_present_rgb565(s_fb, EDGEAI_LCD_W, EDGEAI_LCD_H);
}

const uint16_t *display_hal_framebuffer(void)
{
    return s_fb;
}

uint32_t display_hal_width(void)
{
    return EDGEAI_LCD_W;
}

uint32_t display_hal_height(void)
{
    return EDGEAI_LCD_H;
}
