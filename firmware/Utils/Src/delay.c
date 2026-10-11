#include "Utils/Inc/delay.h"

void HAL_Delay(uint32_t ms) {
    for (uint32_t i = HAL_GetTick(); HAL_GetTick() - i < ms;) {
        ;
    }
}

void mcu_utils_delay_ms(uint32_t ms, bool is_rtos) {
    if (is_rtos) {
        osDelay(ms);
    } else {
        HAL_Delay(ms);
    }
}

void mcu_utils_delay_us(uint32_t us) {
    if ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) == 0) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }

    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000U);
    while ((uint32_t)(DWT->CYCCNT - start) < cycles) {
        ;
    }
}
