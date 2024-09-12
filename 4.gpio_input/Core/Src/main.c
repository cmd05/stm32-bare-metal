#include "stm32f4xx.h"
#include <stdint.h>

// Enable clock access
#define GPIOAEN		(1U << 0)
#define GPIOCEN		(1U << 2)

#define PIN5		(1U << 5)
#define PIN13		(1U << 13)

#define LED_PIN		PIN5
#define BTN_PIN		PIN13

int main() {
	// Enable clock access to GPIOA and GPIOC
	RCC->AHB1ENR |= GPIOAEN; // enable clock access for GPIOA
	RCC->AHB1ENR |= GPIOCEN; // enable clock access for GPIOA

	// set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	// set PC13 (Port C Pin 13]) as input pin
	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);

	while(1) {
		// check if BTN is pressed
		// (note: switch on nucleo f401re board is active low switch)

		if(GPIOC->IDR & BTN_PIN) { // button is ON when unpressed (active low switch)
			// turn on LED
			GPIOA->BSRR = LED_PIN; // "set" bit LED_PIN
		} else {
			// turn off LED
			GPIOA->BSRR = (1U << 21); // "reset" bit LED_PIN
		}
	}
}
