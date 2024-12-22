#include "uart.h"
#include "systick.h"
#include "stm32f4xx.h"

#define GPIOAEN (1U << 0)
#define LED_PIN (1U << 5)

static void uart_rx_callback(void);

int blink_delay = 1000; // ms

int main()
{
    RCC->AHB1ENR |= GPIOAEN;

    GPIOA->MODER &= ~(1U << 11);
    GPIOA->MODER |= (1U << 10);

    uart2_rx_interrupt_init();

    while (1)
    {
        GPIOA->ODR ^= LED_PIN;
        systickDelayMs(blink_delay); // 1 second
    }
}

static void uart_rx_callback(void) {
    char key = USART2->DR; // only first 8 bits of DR are available

    if(key == '1')
        blink_delay = 200;
    else if(key == '2')
        blink_delay = 1000;
    else if(key == '3')
        blink_delay = 3000;
}

// RXNEIE:
// 1: An USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
// All USART interrupts generate the same interrupt USART2_IRQn
void USART2_IRQHandler(void) {
    if(USART2->SR & UART_SR_RXNE) { // we check if it is UART RX interrupt if RXNE (RXNE: Read data register not empty) is set in USART_SR
        uart_rx_callback();
    }
}