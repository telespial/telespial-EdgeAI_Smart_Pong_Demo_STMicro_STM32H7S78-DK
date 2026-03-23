# Build + Flash Guide (STM32H7S78-DK)

Status: validated on hardware, golden and failsafe restore points captured.

## Build
From project root:

./tools/build_arm.sh

## Flash Current Build
./tools/flash_first.sh

## Flash Golden Restore
./tools/flash_restore_golden.sh

## Flash Failsafe Active
./tools/flash_restore_active.sh

## Direct CLI Equivalent
STM32_Programmer_CLI -c port=SWD mode=UR -w stm32_port/build-arm/edgeai_smart_pong_stm32.elf -v -rst

## Validation Checklist
- [x] ARM build completes
- [x] Flash + verify + reset succeeds
- [x] Game renders on LCD
- [x] Golden restore image present in failsafe/
- [x] Failsafe active image present in failsafe/
