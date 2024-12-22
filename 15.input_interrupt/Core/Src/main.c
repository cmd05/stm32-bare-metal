#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "systick.h"
#include "uart.h"
#include "adc.h"
#include "tim.h"
#include "exti.h"

uint32_t sensor_value;

#define GPIOAEN		(1U << 0)
#define GPIOA_5		(1U << 5)

#define LED_PIN		GPIOA_5

static void exti_callback(void);

int main() {
	// Initialize LED
	RCC->AHB1ENR |= GPIOAEN;

	// set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	pc13_exti_init();
	uart2_tx_init();

	while(1) {

	}
}

static void exti_callback(void) {
	printf("BTN pressed. \n\r");
	GPIOA->ODR ^= LED_PIN;
}

// EXTI_PR:
// Bits 22:0 PRx: Pending bit
// 0: No trigger request occurred
// 1: selected trigger request occurred
	// This bit is set when the selected edge event arrives on the external interrupt line.
	// This bit is cleared by programming it to '1'
void EXTI15_10_IRQHandler(void) {
	if((EXTI->PR & EXTI_LINE13) != 0) {
		// Clear PR Flag
		EXTI->PR &= EXTI_LINE13;

		// do something
		exti_callback();
	}
}
