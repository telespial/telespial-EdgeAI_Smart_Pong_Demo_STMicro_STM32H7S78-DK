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
