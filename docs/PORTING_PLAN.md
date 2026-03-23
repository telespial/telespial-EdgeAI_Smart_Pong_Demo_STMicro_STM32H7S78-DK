# Porting Plan - NXP Baseline to STM32H7S78-DK

## Phase 0 - Workspace and Baseline Freeze
- Keep NXP import as frozen reference.
- Define STM32 target structure and toolchain flow.

## Phase 1 - STM32 Platform Skeleton
- Create STM32 project scaffold (startup, clocks, linker, debug UART).
- Add deterministic timing primitives (`time_hal` equivalent).

## Phase 2 - Display Bring-Up
- Implement display init + framebuffer present path.
- Validate fill/rect/blit primitives used by software renderer.

## Phase 3 - Touch/Input Bring-Up
- Implement touch polling and normalize coordinates.
- Map buttons/aux inputs and verify UI navigation.

## Phase 4 - Gameplay Integration
- Integrate `game/*` and rendering loop.
- Validate frame pacing and gameplay physics parity.

## Phase 5 - Persistence + Audio
- Port flash persistence backend.
- Port or stub audio events, then restore full behavior.

## Phase 6 - AI Path
- Keep CPU analytic fallback first.
- Add optional model runtime path only after stable gameplay.

## Exit Criteria
- Stable boots and playable game on STM32H7S78-DK.
- Touch + display + settings + save/restore functionally verified.
- Reproducible build and flash process documented.
