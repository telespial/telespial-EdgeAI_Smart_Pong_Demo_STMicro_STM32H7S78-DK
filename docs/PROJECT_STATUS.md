# Project Status - STM32H7S78-DK Smart Pong Port

## Snapshot
- Date: 2026-03-24
- State: running on STM32H7S78-DK, fairness-tuned high-speed AI with switchable CMSIS-DSP path (default OFF) and promoted golden/failsafe restore points
- Baseline source: EdgeAI_smart_pong_demo_NXP_FRDM-MCXN947
- STM32 implementation: stm32_port/

## Current Milestone
- M10: Fair mixed-mode control + EdgeAI win-tune + UI readability update promoted to golden/failsafe

## Achieved
- Build + flash loop stable via tools/build_arm.sh and tools/flash_first.sh
- Display and gameplay active on target board
- UI tuned for STM32 LCD sizing
- Top-left accelerometer widget and related path removed
- Settings label updated from `NPU` to `EDGEAI`
- Telemetry line updated to LxxxxUS MxxxxUS
- CPU predictor backend enabled in src/platform/npu_hal_stub.c (non-hardware-NPU path)
- AI tuned so ALGO and EDGEAI use same base control math; EDGEAI is additive correction layer only
- Startup serve pace increased by 50%
- High-speed EdgeAI cadence/headroom tuning added for faster rallies
- STM32 event audio added (wall, paddle, point, win) with in-menu `VOL` setting
- Added user-facing `DSP` setting (`ON/OFF`) and wired CMSIS-DSP kernels into EdgeAI disagreement/confidence math
- Updated startup default for `DSP` to `OFF`
- Default SKILL startup set to `ALGO/AI` (left ALGO, right EdgeAI)
- Mixed-mode fairness closeout: removed EdgeAI-only profile boosts/cadence advantages vs ALGO
- EdgeAI win-tuning pass applied without ALGO-path changes
- Increased top side-role text size (`ALGO`, `HUMAN`, `EdgeAI`)
- Latest promoted restore timestamp: `20260324T164436Z`
- Golden + failsafe artifacts captured in failsafe/

## Next Milestone
- M11: Touch robustness closeout and formal smoke checklist signoff

## Current Artifacts
- Current ELF: stm32_port/build-arm/edgeai_smart_pong_stm32.elf
- Current BIN: stm32_port/build-arm/edgeai_smart_pong_stm32.bin
- Golden + failsafe bundle: failsafe/
