#include "par_lcd_s035.h"

#include <stdint.h>

#include "edgeai_config.h"
#include "platform/display_hal.h"

void par_lcd_s035_fill_rect(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t rgb565)
{
    if (x1 < x0 || y1 < y0) return;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= EDGEAI_LCD_W) x1 = EDGEAI_LCD_W - 1;
    if (y1 >= EDGEAI_LCD_H) y1 = EDGEAI_LCD_H - 1;
    if (x1 < x0 || y1 < y0) return;

    uint32_t w = (uint32_t)((x1 - x0) + 1);
    uint32_t h = (uint32_t)((y1 - y0) + 1);

    static uint16_t line[EDGEAI_LCD_W];
    for (uint32_t i = 0; i < w; i++) line[i] = rgb565;

    for (uint32_t r = 0; r < h; r++)
    {
        display_hal_blit_rect(x0, y0 + (int32_t)r, x1, y0 + (int32_t)r, line);
    }
}
