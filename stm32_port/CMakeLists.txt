cmake_minimum_required(VERSION 3.20)
project(edgeai_smart_pong_stm32_port C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(EDGEAI_ARM_GCC "Build bare-metal ARM image for STM32H7S78" OFF)

add_executable(edgeai_smart_pong_stm32
    src/main.c
    src/stm32_platform_bsp.c
    src/sw_render.c
    src/text5x7.c
    src/par_lcd_s035.c
    src/game/ai.c
    src/game/game.c
    src/game/input.c
    src/game/modes.c
    src/game/physics.c
    src/game/render.c
    src/platform/audio_hal.c
    src/platform/display_hal.c
    src/platform/input_hal.c
    src/platform/npu_hal_stub.c
    src/platform/time_hal.c
    src/platform/touch_hal.c
    third_party/stm32cubeh7rs/Drivers/CMSIS/Device/ST/STM32H7RSxx/Source/Templates/system_stm32h7rsxx.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_cortex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_dma.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_dma_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_dma2d.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_exti.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_flash.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_flash_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_gpio.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_i2c.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_i2c_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_i2s.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_i2s_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_lptim.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_ltdc.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_ltdc_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_mdf.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_pwr.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_pwr_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_rcc.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_rcc_ex.c
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Src/stm32h7rsxx_hal_xspi.c
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK/stm32h7s78_discovery_lcd.c
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK/stm32h7s78_discovery_audio.c
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK/stm32h7s78_discovery_bus.c
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK/stm32h7s78_discovery_ts.c
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK/stm32h7s78_discovery_xspi.c
    third_party/stm32cubeh7rs/Drivers/BSP/Components/aps256xx/aps256xx.c
    third_party/stm32cubeh7rs/Drivers/BSP/Components/gt911/gt911.c
    third_party/stm32cubeh7rs/Drivers/BSP/Components/gt911/gt911_reg.c
    third_party/stm32cubeh7rs/Drivers/BSP/Components/wm8904/wm8904.c
    third_party/stm32cubeh7rs/Drivers/BSP/Components/wm8904/wm8904_reg.c
    third_party/stm32cubeh7rs/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_abs_f32.c
    third_party/stm32cubeh7rs/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.c
)

if(EDGEAI_ARM_GCC)
  enable_language(ASM)
  set_target_properties(edgeai_smart_pong_stm32 PROPERTIES SUFFIX ".elf")
  target_sources(edgeai_smart_pong_stm32 PRIVATE
      third_party/stm32cubeh7rs/Drivers/CMSIS/Device/ST/STM32H7RSxx/Source/Templates/gcc/startup_stm32h7s7xx.s
  )
endif()

target_include_directories(edgeai_smart_pong_stm32 PRIVATE
    src
    src/game
    src/platform
    third_party/stm32cubeh7rs/config
    third_party/stm32cubeh7rs/Drivers/BSP/STM32H7S78-DK
    third_party/stm32cubeh7rs/Drivers/BSP/Components/Common
    third_party/stm32cubeh7rs/Drivers/BSP/Components/rk050hr18
    third_party/stm32cubeh7rs/Drivers/BSP/Components/aps256xx
    third_party/stm32cubeh7rs/Drivers/BSP/Components/gt911
    third_party/stm32cubeh7rs/Drivers/BSP/Components/wm8904
    third_party/stm32cubeh7rs/Drivers/BSP/Components/mx25uw25645g
    third_party/stm32cubeh7rs/Drivers/BSP/Components/mx66uw1g45g
    third_party/stm32cubeh7rs/Drivers/STM32H7RSxx_HAL_Driver/Inc
    third_party/stm32cubeh7rs/Drivers/CMSIS/Include
    third_party/stm32cubeh7rs/Drivers/CMSIS/DSP/Include
    third_party/stm32cubeh7rs/Drivers/CMSIS/Device/ST/STM32H7RSxx/Include
)

# CPU-only baseline during early STM32 bring-up.
target_compile_definitions(edgeai_smart_pong_stm32 PRIVATE
    CONFIG_EDGEAI_USE_NPU=0
    USE_HAL_DRIVER
    STM32H7S7xx
    ARM_MATH_CM7
    USE_NOR_MEMORY_MX66UW1G45G=0
    USE_RAM_MEMORY_APS256XX=1
)

if(NOT MSVC)
  target_link_libraries(edgeai_smart_pong_stm32 PRIVATE m)
endif()

if(EDGEAI_ARM_GCC)
  target_compile_options(edgeai_smart_pong_stm32 PRIVATE
    -Os
    -mcpu=cortex-m7
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -ffunction-sections
    -fdata-sections
    -fno-common
    -Wall
    -Wextra
    -Wno-unused-parameter
  )

  target_link_options(edgeai_smart_pong_stm32 PRIVATE
    -mcpu=cortex-m7
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -T${CMAKE_CURRENT_SOURCE_DIR}/linker/stm32h7s78_flash.ld
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,-Map=${CMAKE_BINARY_DIR}/edgeai_smart_pong_stm32.map
    -Wl,--gc-sections
  )

  add_custom_command(TARGET edgeai_smart_pong_stm32 POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:edgeai_smart_pong_stm32>
            ${CMAKE_BINARY_DIR}/edgeai_smart_pong_stm32.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:edgeai_smart_pong_stm32>
  )
endif()
