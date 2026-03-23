#include "platform/touch_hal.h"

#include <string.h>

static bool s_touch_ok = false;

__attribute__((weak)) bool stm32_touch_read(edgeai_touch_state_t *out)
{
    (void)out;
    return false;
}

void touch_hal_init(void)
{
    s_touch_ok = true;
}

void touch_hal_poll(edgeai_touch_state_t *out)
{
    if (!out) return;
    memset(out, 0, sizeof(*out));

    if (!s_touch_ok) return;

    if (!stm32_touch_read(out))
    {
        out->count = 0u;
    }
}

bool touch_hal_is_ok(void)
{
    return s_touch_ok;
}
