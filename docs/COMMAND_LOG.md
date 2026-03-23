# Command Log - STM32 Smart Pong Port

## 2026-03-22
- Built and flashed latest STM32 firmware successfully.
- Updated UI and telemetry behavior on target build.
- Synced core color palette with Infineon Smart Pong look.
- Enabled CPU-based predictor path in src/platform/npu_hal_stub.c and enforced telemetry timing floor (>=1us).
- Captured restore artifacts into failsafe/:
  - edgeai_smart_pong_stm32_golden_20260323T001308Z.{elf,bin}
  - edgeai_smart_pong_stm32_failsafe_active.{elf,bin}
- Added restore automation scripts:
  - tools/flash_restore_golden.sh
  - tools/flash_restore_active.sh
- Updated docs for golden and failsafe workflow.

## Template
- Date:
- Command:
- Result:
- Notes:

## 2026-03-23
- Promoted current tuned STM32 build to golden and failsafe restore points.
- Updated restore pointer docs and flash_restore_golden script to latest golden timestamp.
- Regenerated failsafe SHA256 checksums.

## 2026-03-23 (Promotion refresh)
- Promoted latest aggressive EdgeAI tuning build to new golden timestamp: `20260323T004007Z`.
- Refreshed `failsafe_active` to match the same build output.
- Updated restore-pointer docs/scripts and regenerated `failsafe/SHA256SUMS.txt`.

## 2026-03-23 (Fairness + speed baseline)
- Removed top-left accelerometer mini-widget and associated accel path.
- Updated settings UI label from `NPU` to `EDGEAI` while preserving ON/OFF control behavior.
- Reworked AI architecture for fairness: ALGO and EDGEAI now share the same base control math.
- EDGEAI now acts as a bounded additive prediction-correction layer over analytic intercept.
- Increased startup serve speed by 50%.
- Promoted this build to golden timestamp: `20260323T135322Z`.
- Refreshed `failsafe_active`, restore-pointer docs/scripts, and `failsafe/SHA256SUMS.txt`.

## 2026-03-23 (Audio + latest restore promotion)
- Added STM32 board audio path with event tones (wall, paddle, point, win).
- Added settings `VOL` control (`VOL DN`/`UP`, 0..100, default 60) with live update.
- Tuned EdgeAI high-speed cadence/headroom to improve faster-rally competitiveness without handicapping ALGO baseline math.
- Promoted latest validated build to golden timestamp: `20260323T142009Z`.
- Refreshed `failsafe_active`, restore-pointer docs/scripts, and `failsafe/SHA256SUMS.txt`.

## 2026-03-23 (CMSIS-DSP setting + promotion)
- Added new in-game `DSP` setting (`ON/OFF`) in the settings panel.
- Wired real CMSIS-DSP kernels into AI runtime when `DSP=ON`:
  - `arm_abs_f32`
  - `arm_dot_prod_f32`
  - `arm_sqrt_f32`
- Added scalar fallback path when `DSP=OFF` with same control flow semantics.
- Updated STM32 build wiring for CMSIS-DSP include/compile settings and source linkage.
- Built successfully via `./tools/build_arm.sh`.
- Promoted this build to golden timestamp: `20260323T180311Z`.
- Refreshed `failsafe_active`, restore-pointer docs/scripts, and `failsafe/SHA256SUMS.txt`.
