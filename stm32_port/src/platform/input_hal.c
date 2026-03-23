#include "platform/input_hal.h"

#include <string.h>

#include "edgeai_config.h"

static inline float clamp01f(float v)
{
    if (v < 0.0f) return 0.0f;
    if (v > 1.0f) return 1.0f;
    return v;
}

static inline float remap_touch_y(float ty, float ui_y_max)
{
    if (ty <= ui_y_max) return 0.0f;
    return clamp01f((ty - ui_y_max) / (1.0f - ui_y_max));
}

static inline float remap_touch_z_from_left_edge(float tx)
{
    return clamp01f(tx * (1.0f / EDGEAI_TOUCH_STRIP_W_NORM));
}

static inline float remap_touch_z_from_right_edge(float tx)
{
    float start = 1.0f - EDGEAI_TOUCH_STRIP_W_NORM;
    return clamp01f((tx - start) * (1.0f / EDGEAI_TOUCH_STRIP_W_NORM));
}

bool input_hal_init(input_hal_t *s)
{
    if (!s) return false;
    memset(s, 0, sizeof(*s));
    touch_hal_init();
    return true;
}

void input_hal_poll(input_hal_t *s, platform_input_t *out)
{
    if (!out) return;
    memset(out, 0, sizeof(*out));

    out->p1_y = 0.5f;
    out->p1_z = 0.5f;
    out->p2_y = 0.5f;
    out->p2_z = 0.5f;

    edgeai_touch_state_t ts;
    touch_hal_poll(&ts);
    if (ts.count == 0u)
    {
        if (s) s->prev_touch_active = false;
        return;
    }

    out->touch_active = true;
    out->touch_x = clamp01f(ts.points[0].x);
    out->touch_y = clamp01f(ts.points[0].y);
    out->touch_pressed = s ? (out->touch_active && !s->prev_touch_active) : true;

    const float ui_y_max = (float)EDGEAI_UI_BAR_H / (float)(EDGEAI_LCD_H - 1);

    for (uint32_t i = 0; i < ts.count && i < 2u; i++)
    {
        float tx = clamp01f(ts.points[i].x);
        float ty = clamp01f(ts.points[i].y);
        if (ty < ui_y_max) continue;

        if (tx <= EDGEAI_TOUCH_STRIP_W_NORM)
        {
            out->p1_active = true;
            out->p1_y = remap_touch_y(ty, ui_y_max);
            out->p1_z = remap_touch_z_from_left_edge(tx);
        }
        else if (tx >= (1.0f - EDGEAI_TOUCH_STRIP_W_NORM))
        {
            out->p2_active = true;
            out->p2_y = remap_touch_y(ty, ui_y_max);
            out->p2_z = 1.0f - remap_touch_z_from_right_edge(tx);
        }
    }

    if (s) s->prev_touch_active = out->touch_active;
}
