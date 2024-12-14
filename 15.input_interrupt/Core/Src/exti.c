/*
 * exti.c
 *
 *  Created on: Oct 20, 2024
 *      Author: aquila
 */

#include "exti.h"
#include "stm32f4xx.h"

#define GPIOCEN 		(1U << 2)
#define SYSCFGEN		(1U << 14)
#define EXTI_LINE13		(1U << 13)

void pc13_exti_init(void) {
	// disable global interrupts
	__disable_irq();

	// enable clock access for GPIOC
	RCC->AHB1ENR |= GPIOCEN;

	// set PC13 as input (push button)
	GPIOC->MODER &= ~(1U << 26); // set MODER13
	GPIOC->MODER &= ~(1U << 27);

	// enable clock access for SYSCFG
	RCC->APB2ENR |= SYSCFGEN;

	// Select Port C for EXTI13 (i.e PC13) (0010)
	// SYSCFG_EXTICR4 => SYSCFG->EXTICR[3]
	SYSCFG->EXTICR[3] |= (1U << 5); // note: `SYSCFG_EXTICR4` has reset value: 0x00

	// Unmask EXTI13 (interrupt mask register)
	// 1 => interrupt request from line is not masked
	EXTI->IMR |= EXTI_LINE13;

	// Select Falling edge trigger
	EXTI->FTSR |= EXTI_LINE13;

	// enable EXTI13 line in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn); // or for ex: TIM2_IRQn
	// // NVIC_ISER0 - NVIC_ISER7: NVIC's Interrupt Set-Enable Register (ISER)
    // NVIC->ISER[(((uint32_t)EXTI15_10_IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)EXTI15_10_IRQn) & 0x1FUL));

	// enable global interrupts
	__enable_irq();
}

