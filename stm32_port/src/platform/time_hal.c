#include "platform/time_hal.h"

#include <stdint.h>

#if __has_include("stm32h7rsxx_hal.h")
#include "stm32h7rsxx_hal.h"
#define EDGEAI_HAS_ST_HAL 1
#elif __has_include("stm32h7xx_hal.h")
#include "stm32h7xx_hal.h"
#define EDGEAI_HAS_ST_HAL 1
#else
#define EDGEAI_HAS_ST_HAL 0
#endif

#if defined(__unix__) || defined(__APPLE__)
#include <time.h>
#endif

static uint32_t s_core_hz = 0u;

void time_hal_init(void)
{
#if defined(CoreDebug) && defined(DWT)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0u;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#endif

#if EDGEAI_HAS_ST_HAL
    s_core_hz = HAL_RCC_GetHCLKFreq();
#endif
}

uint32_t time_hal_cycles(void)
{
#if defined(DWT)
    return DWT->CYCCNT;
#else
    return 0u;
#endif
}

uint32_t time_hal_cycles_to_us(uint32_t cycles)
{
    uint32_t hz = s_core_hz;
    if (hz == 0u)
    {
#if EDGEAI_HAS_ST_HAL
        hz = HAL_RCC_GetHCLKFreq();
#else
        hz = 1u;
#endif
    }
    return (uint32_t)(((uint64_t)cycles * 1000000ull) / (uint64_t)hz);
}

uint32_t time_hal_elapsed_us(uint32_t start_cycles)
{
#if defined(DWT)
    uint32_t now = DWT->CYCCNT;
    return time_hal_cycles_to_us(now - start_cycles);
#else
    (void)start_cycles;
    return 0u;
#endif
}

void time_hal_delay_us(uint32_t us)
{
    if (us == 0u) return;

#if EDGEAI_HAS_ST_HAL
    /* Coarse delay for now; replace with timer-based microsecond delay if needed. */
    uint32_t ms = (us + 999u) / 1000u;
    if (ms == 0u) ms = 1u;
    HAL_Delay(ms);
#elif defined(__unix__) || defined(__APPLE__)
    struct timespec ts;
    ts.tv_sec = (time_t)(us / 1000000u);
    ts.tv_nsec = (long)((us % 1000000u) * 1000u);
    nanosleep(&ts, 0);
#else
    volatile uint32_t n = us * 8u;
    while (n--) { __asm__ volatile ("nop"); }
#endif
}
