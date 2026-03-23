# Smart Pong (STM32H7S78-DK)

Smart Pong is a 3D-look Pong implementation ported to STM32H7S78-DK.

## Real World AI Learning Demonstration
Compare three control behaviors in one system:
- `HUMAN`: touch-driven paddle control
- `ALGO`: fixed classic CPU predictor (non-learning baseline)
- `EdgeAI`: adaptive predictor path with runtime learning and telemetry

Observe online learning in live play:
- Adaptive control updates from rally outcomes (hits/misses)
- Runtime profile tuning adjusts paddle speed, anticipation, and tracking noise

SKILL modes allow direct side comparison:
- `AI/ALGO`: left adaptive EdgeAI, right fixed ALGO
- `ALGO/AI`: left fixed ALGO, right adaptive EdgeAI
- `2AI`: both sides adaptive EdgeAI

Persistence behavior:
- `PERSIST ON`: per-side learned profiles carry across new matches and reboot
- `PERSIST OFF`: learned profile is cleared for cold-start behavior

Deployment tradeoff behavior:
- Target has no on-chip NPU, so this STM32 port uses a CPU predictor path
- NPU-oriented hooks and telemetry are retained for future backend integration
- CPU fallback path is always present for robustness

## Hardware Components
Manufacturer: STMicroelectronics  
Name: STM32H7S78 Discovery Kit  
Part number: STM32H7S78-DK

Display and touch are handled using STM32H7S78-DK board support stack (LCD + GT911 TS path in this project).

## Gameplay Rules
- Objective: first side to selected MATCH target (`11`, `100`, `1K`) wins
- Scoring: miss on one side gives opponent +1 point
- Ball motion: reflects from top and bottom arena walls
- Paddle collision: hit-position-based angle shaping
- Serve behavior: starts from center and serves toward side that conceded previous point
- Serve variation: straight, single-axis, and diagonal patterns with receiver-biased direction
- Match flow:
- `P0` (AI vs AI): winner styling + confetti then auto-reset
- `P1` and `P2`: winner styling + confetti then `NEW GAME? YES/NO`
- Absolute cap: at `999`, winner styling + confetti then auto-reset to `0-0` after timeout
- Bottom HUD shows per-side `WINS`

## Controls
- Left touch strip: player 1 paddle
- Right touch strip: player 2 paddle
- Touch Y: paddle height
- Touch X in strip: paddle depth
- Top UI area: settings panel and help
- Alternate control mappings can be routed through `stm32_port/src/platform/input_hal.c`

## Settings
- `Players`: `0`, `1`, `2`
- `Difficulty`: `1`, `2`, `3`
- `NPU`: `ON`, `OFF` (UI compatibility toggle; no hardware NPU on STM32H7S78)
- `SKILL`: `2AI`, `AI/ALGO`, `ALGO/AI`
- `PERSIST`: `ON`, `OFF`
- `MATCH`: `11`, `100`, `1K` (`1K` maps to `999` cap logic)
- `TARGET`: `ON`, `OFF`
- `SPEED++`: `ON`, `OFF`
- `New Game`: immediate score reset and WINS reset to `000`

## AI Runtime In This STM32 Port
Runtime paths:
- CPU analytic intercept predictor (deterministic baseline)
- CPU adaptive predictor path wired through `npu_hal_stub.c` telemetry interface

Behavior:
- Per-side target prediction outputs `(y_hit, z_hit, t_hit)`
- Difficulty applies reaction cadence, speed limits, and noise
- Mixed modes (`AI/ALGO`, `ALGO/AI`) support side-to-side comparison
- Confidence gate blends predictor output against analytic physics
- If predictor path fails, control falls back to analytic path

Important:
- Model-weight retraining is not performed on-device
- Runtime learning adapts per-side control parameters (`speed_scale`, `noise_scale`, `lead_scale`)

## Features
- 3D-look arena with depth cues and wall shading
- `0P` / `1P` / `2P` modes
- Ball-speed-linked color gradient
- New game countdown (`3`, `2`, `1`) on game start
- Fixed-timestep simulation
- Telemetry overlay (`N/F` rates and `L/M` latency fields)
- SKILL side-selectable adaptive AI vs ALGO baseline
- Optional `SPEED++` progression
- AI-driven serve variation with directional bias

## Build and Flash (STM32)
From repo root:

```bash
./tools/build_arm.sh
./tools/flash_first.sh
```

Restore points:

```bash
./tools/flash_restore_golden.sh
./tools/flash_restore_active.sh
```

## Project Layout
- `stm32_port/`: game logic, rendering, platform HAL, AI runtime
- `docs/`: hardware, build/flash, restore-point, and project docs
- `tools/`: build, flash, and restore helper scripts
- `failsafe/`: pinned restore binaries and checksums

## Restore Points
- Active golden/failsafe pointer: `docs/RESTORE_POINTS.md`
- Current golden artifact:
- `failsafe/edgeai_smart_pong_stm32_golden_20260323T004007Z.elf`
- Failsafe active artifact:
- `failsafe/edgeai_smart_pong_stm32_failsafe_active.elf`
