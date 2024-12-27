#include "stm32f4xx.h"
#include "tim.h"
#include "uart.h"

#define LED_PIN     (1U << 5)

void tim2_delay() {
    //	UIF: This bit is set by hardware on an update event. It is cleared by software.
    //	0: No update occurred.
    //	1: Update interrupt pending. This bit is set by hardware when the registers are updated:

    // wait for UIF to be set
    while((TIM2->SR & TIM_SR_UIF) == 0)
        ;

    // clear UIF bit
    TIM2->SR &= ~TIM_SR_UIF;
}

int main() {
    // led initialization
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    uart2_tx_init();
    GPIOA->ODR &= ~LED_PIN; // initially OFF
    
    // tim2_1hz_init();
    // while(1) {
    //     tim2_delay();
    //     printf("A second has passed\n\r");
    //     GPIOA->ODR ^= LED_PIN;
    // }

    tim2_1hz_interrupt_init();
    while(1) {

    }
}

static void tim2_update_callback(void) {
    printf("A second has passed\n\r");
    GPIOA->ODR ^= LED_PIN;
}

void TIM2_IRQHandler(void) {
    if((TIM2->SR & TIM_SR_UIF) != 0) {
        TIM2->SR &= ~TIM_SR_UIF;

        tim2_update_callback();
    }
}