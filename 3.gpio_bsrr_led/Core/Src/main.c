#include "stm32f4xx.h"
#include <stdint.h>

// Enable clock access
#define GPIOAEN		(1U << 0)

#define PIN5		(1U << 5)
#define LED_PIN		PIN5

int main() {
	// enable clock access for GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	while(1) {
			GPIOA->BSRR = LED_PIN; // "set" bit LED_PIN
			for(int i = 0; i < 100000; i++)
				;
			GPIOA->BSRR = (1U << 21); // "reset" bit LED_PIN
			for(int i = 0; i < 100000; i++)
				;
	}
}
