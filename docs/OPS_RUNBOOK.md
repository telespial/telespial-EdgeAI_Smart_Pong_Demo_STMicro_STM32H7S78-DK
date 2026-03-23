# Ops Runbook - STM32 Smart Pong Port

## Fast Recovery
- Restore known-good golden image: ./tools/flash_restore_golden.sh
- Restore last marked failsafe-active image: ./tools/flash_restore_active.sh

## If Build Fails
- Re-run ./tools/build_arm.sh
- Confirm toolchain file exists: stm32_port/cmake/toolchain-arm-none-eabi-gcc.cmake
- Confirm linker script exists: stm32_port/linker/stm32h7s78_flash.ld

## If Flash Fails
- Check ST-LINK connection and board power.
- Retry under-reset mode (already set in scripts).
- Verify artifact path is present in stm32_port/build-arm/.

## If Display Is Blank
- Reflash golden baseline first.
- Verify board reset and power rails.
- Confirm LCD init path in src/stm32_platform_bsp.c.

## If Touch Is Unstable
- Validate GT911 init path and I2C wiring.
- Confirm touch read fallback path in src/stm32_platform_bsp.c.

## Restore Integrity Check
cd failsafe && sha256sum -c SHA256SUMS.txt
