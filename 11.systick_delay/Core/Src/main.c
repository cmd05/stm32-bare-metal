#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "systick.h"
#include "uart.h"
#include "adc.h"

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

	while(1) {
		printf("A second passed\n\r"); // sent from board to computer
		GPIOA->ODR ^= LED_PIN;
		systickDelayMs(1000); // 1 second
	}
}




