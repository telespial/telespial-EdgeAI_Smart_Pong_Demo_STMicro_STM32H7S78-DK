#include <stdbool.h>
#include <stdint.h>

#include "edgeai_config.h"

#include "game/game.h"
#include "game/render.h"
#include "platform/audio_hal.h"
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
    (void)audio_hal_init();

    pong_game_t game;
    game_init(&game);

    render_state_t render;
    render_init(&render);

    const uint32_t frame_us = 1000000u / (uint32_t)EDGEAI_FIXED_FPS;
    const float dt = 1.0f / (float)EDGEAI_FIXED_FPS;
    uint16_t prev_score_left = game.score.left;
    uint16_t prev_score_right = game.score.right;
    bool prev_match_over = game.match_over;
    uint8_t prev_audio_volume = game.audio_volume;
    audio_hal_set_volume(prev_audio_volume);
    uint32_t last_wall_sfx_cycles = 0u;
    uint32_t last_paddle_sfx_cycles = 0u;
    uint32_t last_point_sfx_cycles = 0u;
    const uint32_t wall_sfx_min_gap_us = 50000u;
    const uint32_t paddle_sfx_min_gap_us = 75000u;
    const uint32_t point_sfx_min_gap_us = 150000u;

    for (;;)
    {
        uint32_t start = time_hal_cycles();

        platform_input_t in;
        input_hal_poll(&input, &in);

        game_step(&game, &in, dt);
        if (game.audio_volume != prev_audio_volume)
        {
            audio_hal_set_volume(game.audio_volume);
            prev_audio_volume = game.audio_volume;
        }
        if (game.sfx_wall_bounce_count > 0u)
        {
            uint32_t now = time_hal_cycles();
            if ((last_wall_sfx_cycles == 0u) || (time_hal_elapsed_us(last_wall_sfx_cycles) >= wall_sfx_min_gap_us))
            {
                audio_hal_queue_wall_bounce(1u);
                last_wall_sfx_cycles = now;
            }
            game.sfx_wall_bounce_count = 0u;
        }
        if (game.sfx_paddle_hit_count > 0u)
        {
            uint32_t now = time_hal_cycles();
            if ((last_paddle_sfx_cycles == 0u) || (time_hal_elapsed_us(last_paddle_sfx_cycles) >= paddle_sfx_min_gap_us))
            {
                audio_hal_queue_paddle_hit(1u);
                last_paddle_sfx_cycles = now;
            }
            game.sfx_paddle_hit_count = 0u;
        }
        if ((game.score.left > prev_score_left) || (game.score.right > prev_score_right))
        {
            uint32_t now = time_hal_cycles();
            if ((last_point_sfx_cycles == 0u) || (time_hal_elapsed_us(last_point_sfx_cycles) >= point_sfx_min_gap_us))
            {
                audio_hal_queue_point_scored(1u);
                last_point_sfx_cycles = now;
            }
        }
        if (!prev_match_over && game.match_over)
        {
            audio_hal_queue_win_tune();
        }
        prev_match_over = game.match_over;
        prev_score_left = game.score.left;
        prev_score_right = game.score.right;
        audio_hal_update();
        render_draw_frame(&render, &game);

        uint32_t elapsed_us = time_hal_elapsed_us(start);
        while (elapsed_us < frame_us)
        {
            audio_hal_update();
            uint32_t remaining = frame_us - elapsed_us;
            if (remaining > 300u)
            {
                time_hal_delay_us(200u);
            }
            elapsed_us = time_hal_elapsed_us(start);
        }
    }
}
