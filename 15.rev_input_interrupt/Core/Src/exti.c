/*
 * exti.c
 *
 *  Created on: Dec 14, 2024
 *      Author: aquila
 */

#include "stm32f4xx.h"

#define GPIOCEN		(1U << 2)
#define SYSCFGEN	(1U << 14)

void pc13_exti_init(void) {
	// disable global interrupts
	__disable_irq();

	// enable clock access for GPIOC
	RCC->AHB1ENR |= GPIOCEN;

	// set PC13 as input
	GPIOC->MODER &= ~(1U << 26) & ~(1U << 27);

	// enable clock access to SYSCFG
	RCC->APB2ENR |= SYSCFGEN;

	// select port C for EXTI13 i.e use PC13 for EXTI13 interrupt
	SYSCFG->EXTICR[3] |= (1U << 5);

	// unmask EXTI13
	EXTI->IMR |= (1U << 13);

	// select falling edge trigger for pin 13
	EXTI->FTSR |= (1U << 13);

	// enable exti13 line in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	// enable global interrupts
	__enable_irq();
}
