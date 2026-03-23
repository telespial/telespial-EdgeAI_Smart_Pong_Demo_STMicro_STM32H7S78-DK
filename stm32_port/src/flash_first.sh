#!/usr/bin/env bash
set -euo pipefail

ELF_PATH="${1:-build-arm/edgeai_smart_pong_stm32.elf}"
CLI="/usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"

if [[ ! -f "$ELF_PATH" ]]; then
  echo "ELF not found: $ELF_PATH" >&2
  exit 1
fi

"$CLI" -c port=SWD mode=UR -w "$ELF_PATH" -v -rst
