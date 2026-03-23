# START HERE - STM32H7S78-DK Port

## Goal
Run, maintain, and recover the STM32H7S78-DK Smart Pong port with a documented golden baseline and fast restore path.

## First 10 Minutes
- Read root README.md and start_here.md.
- Build firmware: ./tools/build_arm.sh
- Flash current build: ./tools/flash_first.sh
- Confirm restore path: ./tools/flash_restore_golden.sh

## Golden/Failsafe Policy
- failsafe/ stores known-good golden and failsafe_active artifacts.
- Every validated milestone should refresh both restore points and checksums.
- docs/RESTORE_POINTS.md is the source of truth for restore usage.

## Documentation Index
- PROJECT_STATUS.md: current state and milestone
- BUILD_FLASH_STM32H7S78DK.md: build and flash commands
- RESTORE_POINTS.md: golden and failsafe restore workflow
- OPS_RUNBOOK.md: troubleshooting and recovery
- COMMAND_LOG.md: operation history
- HARDWARE_DOCS_INDEX.md: local board, RM, and schematic PDFs
