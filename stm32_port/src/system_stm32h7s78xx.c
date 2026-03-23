#include <stdint.h>

uint32_t SystemCoreClock = 64000000u;

void SystemInit(void)
{
    /* Minimal system init placeholder for early bring-up.
     * Real clock/tree init should be provided by STM32Cube-generated code.
     */
#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    /* Leave VTOR at reset default (FLASH base) for now. */
#endif
}
