#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "uart.h"

#define GPIOAEN		(1U << 0)
#define GPIOA_5		(1U << 5)

#define LED_PIN		GPIOA_5

static void uart_callback(void);

char key;


int main() {
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);


//	uart2_rxtx_init();
//	while(1) {
//		key = uart2_read();
//
//		if(key == '1') {
//			GPIOA->ODR |= LED_PIN;
//		} else {
//			GPIOA->ODR &= ~LED_PIN;
//		}
//
//		// printf("Hello from STM32 F4 :D ...\n\r");
//	}

	uart2_rx_interrupt_init();

	while(1) {

	}
}

static void uart_callback(void) {
	key = USART2->DR;
	if(key == '1') {
		GPIOA->ODR |= LED_PIN;
		printf("ON.\n\r");
	} else {
		GPIOA->ODR &= ~LED_PIN;
		printf("OFF.\n\r");
	}
}

void USART2_IRQHandler(void) {
	// check if RXNE is set
	if(USART2->SR & SR_RXNE) {
		// RXNE type (RXNEIE) interrupt has occured
		// do something
		uart_callback();
	}
}
