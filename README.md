# EdgeAI Smart Pong - STM32H7S78-DK Port Workspace

This workspace contains the STM32H7S78-DK port of Smart Pong and a reproducible build, flash, and recovery flow.

## Current Golden Baseline
- Target: STM32H7S78-DK
- Build: stm32_port/build-arm/edgeai_smart_pong_stm32.elf
- Golden restore bundle: failsafe/
- Status: builds, flashes, and boots on target with current UI and gameplay tuning.

## Quick Start
- Onboarding: start_here.md
- Full guide: docs/START_HERE.md
- Build and flash: docs/BUILD_FLASH_STM32H7S78DK.md
- Restore points: docs/RESTORE_POINTS.md

## Workspace Layout
- stm32_port/: STM32 firmware source and CMake build
- failsafe/: golden and active restore artifacts (.elf/.bin), checksums, notes
- docs/: status, runbooks, hardware references
- tools/: helper scripts for build, flash, and restore
- EdgeAI_smart_pong_demo_NXP_FRDM-MCXN947/: imported reference baseline

## Standard Commands
- ./tools/build_arm.sh
- ./tools/flash_first.sh
- ./tools/flash_restore_golden.sh
