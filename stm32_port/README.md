# STM32 Port Skeleton (Smart Pong)

This folder is the initial STM32H7S78-DK port scaffold.

## What Is Included
- Portable game/render core copied from NXP baseline
- STM32-oriented platform layer stubs under `src/platform/`
- Minimal `main.c` entrypoint for bring-up
- CMake build scaffold for iterative port work

## Current Limitations
- Display present hook is weak stub (`stm32_display_present_rgb565`)
- Touch backend is weak stub (`stm32_touch_read`)
- Flash persistence shim (`fsl_flash.h`) currently returns fail
- NPU path disabled (`CONFIG_EDGEAI_USE_NPU=0`)

## Integration Hooks
Implement these symbols in your STM32 board layer:
- `void stm32_platform_init(void)`
- `void stm32_display_present_rgb565(const uint16_t *fb, uint32_t w, uint32_t h)`
- `bool stm32_touch_read(edgeai_touch_state_t *out)`
