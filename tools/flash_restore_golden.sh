#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
CLI="/usr/local/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI"
ELF="$ROOT_DIR/failsafe/edgeai_smart_pong_stm32_golden_20260323T001308Z.elf"

if [[ ! -f "$ELF" ]]; then
  echo "Missing golden ELF: $ELF" >&2
  exit 1
fi

"$CLI" -c port=SWD mode=UR -w "$ELF" -v -rst
