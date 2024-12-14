#include "stm32f4xx.h"
#include "exti.h"
#include "uart.h"

#include <stdio.h>

#define GPIOAEN 	(1U << 0)
#define LED_PA5		(1U << 5)

int main() {
	pc13_exti_init();
	uart2_rxtx_init();

	RCC->AHB1ENR |= GPIOAEN;

	// set PA5 as output pin
	GPIOA->MODER &= ~(1U << 11);
	GPIOA->MODER |= (1U << 10);

	while(1);
}

static void exti_callback(void) {
	printf("Button Pressed\r\n");
	GPIOA->ODR ^= LED_PA5;
}

void EXTI15_10_IRQHandler(void) {
	// PRx- 1: selected trigger request occurred.
	// This bit is set when the selected edge event arrives on the external interrupt line.
	if((EXTI->PR & LINE13) != 0) {
		// PRx: This bit is cleared by programming it to ‘1’
		EXTI->PR |= LINE13; // clear pending register by setting it to 1

		// do something
		exti_callback();
	}
}
