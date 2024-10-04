#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "systick.h"
#include "uart.h"
#include "adc.h"
#include "tim.h"

uint32_t sensor_value;

#define GPIOAEN		(1U << 0)
#define GPIOA_5		(1U << 5)

#define LED_PIN		GPIOA_5

// Single channel - single conversion mode
int main() {
	// LED initialization
	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	// write data to serial
	uart2_tx_init();

	// initialize tim2 to 1hz
	tim2_1hz_init();

	while(1) {
		//	UIF: This bit is set by hardware on an update event. It is cleared by software.
		//	0: No update occurred.
		//	1: Update interrupt pending. This bit is set by hardware when the registers are updated:

		// wait for UIF to be set
		while((TIM2->SR & SR_UIF) == 0)
			;

		// clear the UIF bit
		TIM2->SR &= ~SR_UIF;

		printf("A second passed\n\r"); // sent from board to computer
		GPIOA->ODR ^= LED_PIN;
	}
}
