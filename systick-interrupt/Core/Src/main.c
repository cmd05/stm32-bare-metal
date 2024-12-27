#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"

#define LED_PIN     (1U << 5)   // PA5

static void systick_interrupt_callback(void);

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    GPIOA->MODER &= ~(1U << 11);
    GPIOA->MODER |= (1U << 10);

    uart2_tx_init();
    systick_1hz_interrupt();

    printf("Hello World\n\r");

    while(1) {

    }
}

static void systick_interrupt_callback(void) {
    GPIOA->ODR ^= LED_PIN;
    printf("A second passed\n\r"); // note: \n\r is necessary for properly printing on serial monitor
}

void SysTick_Handler(void) {
    systick_interrupt_callback();
}