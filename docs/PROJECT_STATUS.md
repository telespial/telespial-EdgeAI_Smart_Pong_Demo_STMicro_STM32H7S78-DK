# Project Status - STM32H7S78-DK Smart Pong Port

## Snapshot
- Date: 2026-03-23
- State: running on STM32H7S78-DK, fairness-tuned AI and refreshed golden/failsafe restore points
- Baseline source: EdgeAI_smart_pong_demo_NXP_FRDM-MCXN947
- STM32 implementation: stm32_port/

## Current Milestone
- M6: Fair-base EdgeAI layer tuning validated and promoted to golden/failsafe

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
- Golden + failsafe artifacts captured in failsafe/

## Next Milestone
- M7: Touch robustness closeout and formal smoke checklist signoff

## Current Artifacts
- Current ELF: stm32_port/build-arm/edgeai_smart_pong_stm32.elf
- Current BIN: stm32_port/build-arm/edgeai_smart_pong_stm32.bin
- Golden + failsafe bundle: failsafe/
