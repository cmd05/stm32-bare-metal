#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "systick.h"
#include "uart.h"

//uint32_t sensor_value;
#define STRLEN(s) (sizeof(s)/sizeof(s[0]))

#define GPIOAEN		(1U << 0)
#define GPIOA_5		(1U << 5)

#define LED_PIN		GPIOA_5

int __io_putchar(int ch) {
    // Wait until TXE (transmit data register empty) is set
    while (!(USART2->SR & SR_TXE));

    // Load character directly into data register
    USART2->DR = (uint8_c) ch;

    return ch;
}

// Single channel - single conversion mode
int main() {
	// LED initialization
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	uart2_tx_interrupt_init();

	while(1) {
		printf("Hello World\n\r");
		USART2->CR1 |= CR1_TXEIE;

		systickDelayMs(1000);
	}

//	// write data to serial
//	uart2_tx_init();
//
//	while(1) {
//		printf("A second passed\n\r"); // sent from board to computer
//		GPIOA->ODR ^= LED_PIN;
//		systickDelayMs(1000); // 1 second
//	}
}

//int __io_putchar(int ch) {
//	uart2_write(ch);
//	return ch;
//}

//static void uart_tx_callback(void) {
//	// toggle led
//	USART2->DR = ('x' & 0xFF); // transmit only 8 bits
//	//	GPIOA->ODR ^= LED_PIN;
//	systickDelayMs(10); // 1
//}


static void uart_tx_callback(void) {
//	USART2->DR = 'x';
	GPIOA->ODR ^= LED_PIN;
}

void USART2_IRQHandler(void) {
	// RXNE interrupt
	if(USART2->SR & SR_RXNE) {
		// do something
	}

	if(USART2->SR & SR_TXE) {
		// do something
		uart_tx_callback();
	}

	if(USART2->SR & SR_TC) {

	}
}
