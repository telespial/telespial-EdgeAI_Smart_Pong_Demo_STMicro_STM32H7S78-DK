#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT_DIR/stm32_port"

if [[ ! -f build-arm/CMakeCache.txt ]]; then
  cmake -S . -B build-arm -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi-gcc.cmake \
    -DEDGEAI_ARM_GCC=ON
fi

cmake --build build-arm -j4
