#include "stm32f4xx.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "systick.h"

#define LED_PIN     (1U << 5)

void init_led() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);

    GPIOA->ODR &= ~LED_PIN; // initially OFF
}

int main() {
    char message[] = "Hello from STM32F401RE!\n\r";
    uint32_t msg_len = strlen(message) + 1; /* including NULL terminator */

    uart2_tx_init();
    // USART_DR can hold 8 bits of data
    // we have a buffer message containing (`strlen(message) + 1`) bytes, which is the buffer length
    dma1_stream6_init((uint32_t) message, (uint32_t) &USART2->DR, msg_len);
    init_led();

    while(1) {
        // printf("DMA_Program_Test: One sec passed\n\r");
        // systickDelayMs(1000);
    }
}

static void dma_stream6_tc_callback(void) {
    // do a 1 second blink on transfer complete
    GPIOA->ODR ^= LED_PIN;
    systickDelayMs(1000);
    GPIOA->ODR ^= LED_PIN;
}

void DMA1_Stream6_IRQHandler(void) {
    // check for transfer complete interrupt
    if((DMA1->HISR & DMA_HISR_TCIF6) != 0) {
        DMA1->HIFCR |= DMA_HIFCR_CTCIF6; // clear TCIF6 interrupt flag
        dma_stream6_tc_callback();
    }
}