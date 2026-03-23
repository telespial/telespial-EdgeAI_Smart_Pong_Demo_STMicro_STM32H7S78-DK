#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CLI="/usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"
ELF="$ROOT_DIR/stm32_port/build-arm/edgeai_smart_pong_stm32.elf"

if [[ ! -f "$ELF" ]]; then
  echo "Missing ELF: $ELF" >&2
  echo "Run tools/build_arm.sh first." >&2
  exit 1
fi

"$CLI" -c port=SWD mode=UR -w "$ELF" -v -rst
