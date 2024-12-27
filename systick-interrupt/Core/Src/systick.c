#include "stm32f4xx.h"

#define ONE_SEC_LOAD            16000000

void systick_1hz_interrupt(void) {
    SysTick->LOAD = ONE_SEC_LOAD - 1;

    SysTick->VAL = 0; // clear systick current value register

    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
    
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}