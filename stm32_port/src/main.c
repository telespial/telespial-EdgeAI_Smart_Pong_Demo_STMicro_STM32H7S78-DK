#include <stdbool.h>
#include <stdint.h>

#include "edgeai_config.h"

#include "game/game.h"
#include "game/render.h"
#include "platform/display_hal.h"
#include "platform/input_hal.h"
#include "platform/time_hal.h"
#include "sw_render.h"
#include "text5x7.h"

__attribute__((weak)) void stm32_platform_init(void)
{
}

static void draw_boot_title(void)
{
    const int32_t scale = 6;
    const char *title_top = "SMART";
    const char *title_bottom = "PONG";
    int32_t w_top = edgeai_text5x7_width(scale, title_top);
    int32_t w_bottom = edgeai_text5x7_width(scale, title_bottom);

    const int32_t line_h = 7 * scale;
    const int32_t line_gap = 10;
    const int32_t total_h = (2 * line_h) + line_gap;
    int32_t y_top = (EDGEAI_LCD_H - total_h) / 2;
    int32_t y_bottom = y_top + line_h + line_gap;

    int32_t x_top = (EDGEAI_LCD_W - w_top) / 2;
    int32_t x_bottom = (EDGEAI_LCD_W - w_bottom) / 2;

    const uint16_t shadow = sw_pack_rgb565_u8(10, 10, 12);
    const uint16_t face = sw_pack_rgb565_u8(214, 215, 217);

    edgeai_text5x7_draw_scaled(x_top + 3, y_top + 3, scale, title_top, shadow);
    edgeai_text5x7_draw_scaled(x_top, y_top, scale, title_top, face);
    edgeai_text5x7_draw_scaled(x_bottom + 3, y_bottom + 3, scale, title_bottom, shadow);
    edgeai_text5x7_draw_scaled(x_bottom, y_bottom, scale, title_bottom, face);
}

int main(void)
{
    stm32_platform_init();
    time_hal_init();

    if (!display_hal_init())
    {
        for (;;) {}
    }

    display_hal_fill(sw_pack_rgb565_u8(8, 18, 48));
    draw_boot_title();

    input_hal_t input;
    (void)input_hal_init(&input);

    pong_game_t game;
    game_init(&game);

    render_state_t render;
    render_init(&render);

    const uint32_t frame_us = 1000000u / (uint32_t)EDGEAI_FIXED_FPS;
    const float dt = 1.0f / (float)EDGEAI_FIXED_FPS;

    for (;;)
    {
        uint32_t start = time_hal_cycles();

        platform_input_t in;
        input_hal_poll(&input, &in);

        game_step(&game, &in, dt);
        render_draw_frame(&render, &game);

        uint32_t elapsed_us = time_hal_elapsed_us(start);
        if (elapsed_us < frame_us)
        {
            time_hal_delay_us(frame_us - elapsed_us);
        }
    }
}
