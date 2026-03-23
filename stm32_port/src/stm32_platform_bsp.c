#include <stdint.h>

#include "edgeai_config.h"
#include "platform/touch_hal.h"
#include "stm32h7rsxx_hal.h"
#include "stm32h7s78_discovery_lcd.h"
#include "stm32h7s78_discovery_ts.h"

static bool s_ts_ready = false;

static bool stm32_touch_try_init(void)
{
    TS_Init_t ts_init = {0};
    ts_init.Width = EDGEAI_LCD_W;
    ts_init.Height = EDGEAI_LCD_H;
    ts_init.Orientation = TS_SWAP_NONE;
    ts_init.Accuracy = 5u;
    return (BSP_TS_Init(0, &ts_init) == BSP_ERROR_NONE);
}

static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    HAL_MPU_Disable();

    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 0x00000000u;
    MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
    MPU_InitStruct.SubRegionDisable = 0x87u;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x70000000u;
    MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
    MPU_InitStruct.SubRegionDisable = 0x00u;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.BaseAddress = 0x90000000u;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};

    if (HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY) != HAL_OK)
    {
        for (;;) {}
    }

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        for (;;) {}
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL1.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL1.PLLM = 4;
    RCC_OscInitStruct.PLL1.PLLN = 25;
    RCC_OscInitStruct.PLL1.PLLP = 1;
    RCC_OscInitStruct.PLL1.PLLQ = 1;
    RCC_OscInitStruct.PLL1.PLLR = 1;
    RCC_OscInitStruct.PLL1.PLLS = 1;
    RCC_OscInitStruct.PLL1.PLLT = 1;
    RCC_OscInitStruct.PLL1.PLLFractional = 0;
    RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        for (;;) {}
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_PCLK4 | RCC_CLOCKTYPE_PCLK5;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        for (;;) {}
    }
}

void stm32_platform_init(void)
{
    MPU_Config();
    SCB_EnableICache();

    HAL_Init();
    SystemClock_Config();

    if (BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565, 800, 480) != BSP_ERROR_NONE)
    {
        for (;;) {}
    }

    (void)BSP_LCD_SetActiveLayer(0, 0);
    (void)BSP_LCD_SetLayerAddress(0, 0, 0x90000000u);
    (void)BSP_LCD_SetTransparency(0, 0, 255);
    (void)BSP_LCD_SetBrightness(0, 40);
    (void)BSP_LCD_DisplayOn(0);
    (void)BSP_LCD_FillRect(0, 0, 0, 800, 480, LCD_COLOR_RGB565_BLUE);

    s_ts_ready = stm32_touch_try_init();
}

void stm32_display_present_rgb565(const uint16_t *fb, uint32_t w, uint32_t h)
{
    (void)fb;
    (void)w;
    (void)h;
    (void)BSP_LCD_Reload(0, BSP_LCD_RELOAD_IMMEDIATE);
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

bool stm32_touch_read(edgeai_touch_state_t *out)
{
    TS_State_t ts1 = {0};
    TS_MultiTouch_State_t ts = {0};
    uint32_t n = 0;

    if (out == NULL)
    {
        return false;
    }

    if (!s_ts_ready)
    {
        s_ts_ready = stm32_touch_try_init();
        if (!s_ts_ready) return false;
    }

    if (BSP_TS_Get_MultiTouchState(0, &ts) == BSP_ERROR_NONE)
    {
        n = ts.TouchDetected;
        if (n > 2u) n = 2u;
        out->count = n;

        for (uint32_t i = 0; i < n; ++i)
        {
            float xn = (float)ts.TouchX[i] / (float)(EDGEAI_LCD_W - 1);
            float yn = (float)ts.TouchY[i] / (float)(EDGEAI_LCD_H - 1);

            if (xn < 0.0f) xn = 0.0f; else if (xn > 1.0f) xn = 1.0f;
            if (yn < 0.0f) yn = 0.0f; else if (yn > 1.0f) yn = 1.0f;

            out->points[i].active = true;
            out->points[i].x = xn;
            out->points[i].y = yn;
            out->points[i].id = (uint8_t)i;
        }

        if (n > 0u) return true;
    }

    /* Fallback for panels/controllers that only report stable single-touch in this mode. */
    if (BSP_TS_GetState(0, &ts1) != BSP_ERROR_NONE)
    {
        return false;
    }

    if (ts1.TouchDetected != 0u)
    {
        float xn = (float)ts1.TouchX / (float)(EDGEAI_LCD_W - 1);
        float yn = (float)ts1.TouchY / (float)(EDGEAI_LCD_H - 1);

        if (xn < 0.0f) xn = 0.0f; else if (xn > 1.0f) xn = 1.0f;
        if (yn < 0.0f) yn = 0.0f; else if (yn > 1.0f) yn = 1.0f;

        out->count = 1u;
        out->points[0].active = true;
        out->points[0].x = xn;
        out->points[0].y = yn;
        out->points[0].id = 0u;
        return true;
    }

    return false;
}
