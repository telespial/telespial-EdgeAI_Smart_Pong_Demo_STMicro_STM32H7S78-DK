#pragma once

#include <stdint.h>

/* Display geometry for STM32H7S78-DK onboard panel. */
#ifndef EDGEAI_LCD_W
#define EDGEAI_LCD_W 800
#endif

#ifndef EDGEAI_LCD_H
#define EDGEAI_LCD_H 480
#endif

/* Render tile limits. Keep memory footprint moderate while preserving throughput. */
#ifndef EDGEAI_TILE_MAX_W
#define EDGEAI_TILE_MAX_W 200
#endif

#ifndef EDGEAI_TILE_MAX_H
#define EDGEAI_TILE_MAX_H 120
#endif

/* Fixed-timestep target. */
#ifndef EDGEAI_FIXED_FPS
#define EDGEAI_FIXED_FPS 180
#endif

/* UI top bar height. */
#ifndef EDGEAI_UI_BAR_H
#define EDGEAI_UI_BAR_H 40
#endif

/* Touch control strips at left/right edges (normalized width). */
#ifndef EDGEAI_TOUCH_STRIP_W_NORM
#define EDGEAI_TOUCH_STRIP_W_NORM 0.15f
#endif
