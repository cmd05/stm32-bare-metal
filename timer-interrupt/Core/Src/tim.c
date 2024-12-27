#include "stm32f4xx.h"

// initialize TIM2 to perform a 1hz update event
// i.e 1 second delay

void tim2_1hz_init(void) {
    // enable clock access to tim2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // set prescaler value
    // since we have not configured clock tree
	// APB1 bus is at 16 Mhz which is the default clock speed of the processor
    
    TIM2->PSC = 1600 - 1; // 16,000,000 (APB1 clock) / 1600 (TIM2->PSC value) = 10,000

    // set auto reload value
    TIM2->ARR = 10000 - 1; // 10,000 (value after psc division) / 10,000 (TIM2->ARR value) => 1hz update event

    // clear counter
    TIM2->CNT = 0;

    // enable timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

void tim2_1hz_interrupt_init(void) {
    // enable clock access to tim2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // set prescaler value
    // since we have not configured clock tree
	// APB1 bus is at 16 Mhz which is the default clock speed of the processor
    
    TIM2->PSC = 1600 - 1; // 16,000,000 (APB1 clock) / 1600 (TIM2->PSC value) = 10,000

    // set auto reload value
    TIM2->ARR = 10000 - 1; // 10,000 (value after psc division) / 10,000 (TIM2->ARR value) => 1hz update event

    // clear counter
    TIM2->CNT = 0;

    // enable timer interrupt
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);

    // enable timer
    TIM2->CR1 |= TIM_CR1_CEN;
}