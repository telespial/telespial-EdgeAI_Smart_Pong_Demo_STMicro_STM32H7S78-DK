# Hardware Setup - STM32H7S78-DK

## Board
- Target: STM32H7S78-DK
- Display: onboard 800x480 panel
- Touch: onboard capacitive touch controller (I2C path)

## Bring-Up Order
1. Power and ST-LINK debug connection
2. Basic UART log output
3. Display clock + layer init and solid color test
4. Touch polling and coordinate print
5. Game frame loop integration

## Port Mapping Notes
Map NXP platform interfaces to STM32 equivalents:
- `display_hal`: LTDC/DSI or board display stack
- `touch_hal`: touch controller driver + normalized coordinates
- `input_hal`: touch/buttons/optional sensor path
- `time_hal`: DWT cycle counter + microsecond delay
- persistence: flash sector read/write backend
