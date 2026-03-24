# Smart Pong (STM32H7S78-DK)

![STM32H7S78](https://github.com/user-attachments/assets/adefe2a6-9e2a-43e5-b325-02e0099572a1)

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
- `EDGEAI`: `ON`, `OFF` (UI label; no hardware NPU on STM32H7S78)
- `DSP`: `ON`, `OFF` (`ON` enables CMSIS-DSP kernels in the EdgeAI path; startup default is `OFF`)
- `SKILL`: `2AI`, `AI/ALGO`, `ALGO/AI` (startup default: `ALGO/AI`)
- `PERSIST`: `ON`, `OFF`
- `MATCH`: `11`, `100`, `1K` (`1K` maps to `999` cap logic)
- `TARGET`: `ON`, `OFF`
- `SPEED++`: `ON`, `OFF`
- `VOL`: `VOL DN` / `UP` in 5% steps (`0..100`, default `60`)
- `New Game`: immediate score reset and WINS reset to `000`

## AI Runtime In This STM32 Port
Runtime paths:
- CPU analytic intercept predictor (deterministic baseline)
- CPU adaptive predictor path wired through `npu_hal_stub.c` telemetry interface

Behavior:
- Per-side target prediction outputs `(y_hit, z_hit, t_hit)`
- ALGO remains the fixed analytic baseline path
- EDGEAI applies an additive prediction-correction layer on top of analytic intercept
- EdgeAI-side tuning can adjust correction strength, refresh cadence, and movement/noise assists while ALGO logic remains untouched
- Mixed modes (`AI/ALGO`, `ALGO/AI`) support side-to-side comparison
- Confidence gate bounds predictor correction against analytic physics
- If predictor path fails, control falls back to analytic path

Important:
- Model-weight retraining is not performed on-device
- Runtime learning adapts per-side control parameters (`speed_scale`, `noise_scale`, `lead_scale`)

## DSP and EdgeAI Implementation Details (STM32H7S78-DK)
This port currently runs all AI math on the Cortex-M7 CPU (`CONFIG_EDGEAI_USE_NPU=0`), with NPU-facing HAL hooks retained for forward compatibility.

Current DSP-style compute in this build:
- Floating-point physics/intercept math at fixed timestep on M7 FPU.
- EdgeAI correction layer computes bounded deltas over the analytic intercept, with disagreement/confidence gating before blend.
- `DSP ON` path uses CMSIS-DSP kernels in `ai.c` (`arm_abs_f32`, `arm_dot_prod_f32`, `arm_sqrt_f32`) for disagreement/confidence math.
- `DSP OFF` path uses scalar C math fallback for identical gameplay flow without CMSIS-DSP kernels.
- Runtime telemetry smoothing in the NPU HAL stub (`last_infer_us`, moving average `avg_infer_us`) and on-screen `N/F` + `L/M` reporting.
- Audio synthesis path uses deterministic real-time DSP primitives in `audio_hal.c`: phase-accumulator oscillator, event-tone queueing, DMA half/full refill, and fixed-point (`Q12`) volume gain mapping.

What is not enabled yet in this STM32 branch:
- No on-chip NPU execution (STM32H7S78 has no integrated NPU).
- No on-device model-weight training; learning remains runtime control-parameter adaptation.

Future acceleration hook points:
- `src/platform/npu_hal_stub.c` and `src/game/ai.c` define the prediction interface and fallback flow.
- These files are the insertion points for an external accelerator backend (or optional CMSIS-DSP kernelization) while preserving current ALGO/EDGEAI behavior and telemetry semantics.

## Features
- 3D-look arena with depth cues and wall shading
- `0P` / `1P` / `2P` modes
- Ball-speed-linked color gradient
- New game countdown (`3`, `2`, `1`) on game start
- Fixed-timestep simulation
- Telemetry overlay (`N/F` rates and `L/M` latency fields)
- SKILL side-selectable adaptive AI vs ALGO baseline
- Optional `SPEED++` progression
- Event SFX audio on STM32H7S78-DK (wall, paddle, score, win tune)
- In-menu volume control (`VOL DN`/`UP`) with live audio update
- AI-driven serve variation with directional bias
- Startup serve speed increased by 50% from prior baseline
- Larger top side-role labels (`ALGO`, `HUMAN`, `EdgeAI`) for readability

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
- `failsafe/edgeai_smart_pong_stm32_golden_20260324T164436Z.elf`
- Failsafe active artifact:
- `failsafe/edgeai_smart_pong_stm32_failsafe_active.elf`
