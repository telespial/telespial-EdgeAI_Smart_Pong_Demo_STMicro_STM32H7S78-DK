# Restore Points - STM32H7S78-DK Smart Pong

## Location
All restore artifacts live in failsafe/.

## Current Golden
- edgeai_smart_pong_stm32_golden_20260323T180746Z.elf
- edgeai_smart_pong_stm32_golden_20260323T180746Z.bin

## Current Failsafe Active
- edgeai_smart_pong_stm32_failsafe_active.elf
- edgeai_smart_pong_stm32_failsafe_active.bin

## Integrity
- Verify with failsafe/SHA256SUMS.txt

## Restore Commands
- ./tools/flash_restore_golden.sh
- ./tools/flash_restore_active.sh

## Policy
- Refresh both golden and failsafe_active after each validated board milestone.
- Keep this file and failsafe/README_GOLDEN.txt synchronized.
