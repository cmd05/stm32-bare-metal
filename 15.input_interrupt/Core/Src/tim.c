/*
 * tim.c
 *
 *  Created on: Sep 29, 2024
 *      Author: aquila
 */


#include "stm32f4xx.h"

#define TIM2EN		(1U << 0)
#define TIM3EN		(1U << 1)

#define CR1_CEN		(1U << 0)
#define OC_TOGGLE	((1U << 4) | (1U << 5))
#define CCER_CC1E	(1U << 0)

#define GPIOAEN		(1U << 0)
#define AFR5_TIM	(1U << 20) // set bit for pin 5
#define AFR6_TIM	(1U << 25)

#define CCMR1_CC1S	(1U << 0)
#define CCER_CC1S	(1U << 0)

// Output comparison: toggle LED at PA5 based on timer value
// initialize TIM2 to perform a 1Hz update event
// i.e 1 second delay
void tim2_pa5_output_compare(void) {
	/* Configure pin PA5 */
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA5 mode to alternate function
	// `10` => alternate function mode
	GPIOA->MODER &= ~(1U << 10);
	GPIOA->MODER |= (1U << 11);

	// set alternate function type to TIM2_CH1 (AF01)
	GPIOA->AFR[0] |= AFR5_TIM;


	/* Configure TIM2 */
	// enable clock access to TIM2
	RCC->APB1ENR |= TIM2EN;

	// set prescaler value
	TIM2->PSC = 1600 - 1; // 16,000,000 (APB1 clock) / 1600 (TIM2->PSC value) = 10,000

	// set auto-reload value
	TIM2->ARR = 10000 - 1; // 10,000 (value after psc division) / 10,000 (TIM2->ARR value) => 1hz update event

	/* Set TIM2 in Output Compare Toggle Mode */
	// enable TIM2 CH1 in output compare mode
	// OC1M: These bits define the behavior of the output reference signal OC1REF from which OC1 and OC1N are derived.
	// 011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
	TIM2->CCMR1 = OC_TOGGLE;

	// enable TIM2 channel 1
	TIM2->CCER |= CCER_CC1E;

	// clear counter
	TIM2->CNT = 0;

	// enable timer
	TIM2->CR1 |= CR1_CEN;
}

void tim3_pa6_input_capture(void) {
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set pa6 mode to alternate function
	GPIOA->MODER &= ~(1U << 12);
	GPIOA->MODER |= (1U << 13);

	// set pa6 alternate function type (AF02)
	GPIOA->AFR[0] |= AFR6_TIM;

	// enable clock access to TIM3
	RCC->APB1ENR |= TIM3EN;

	// set prescaler
	TIM3->PSC = 16000 - 1; // 16,000,000 / 16000

	// set ch1 to input capture
	TIM3->CCMR1 = CCMR1_CC1S;

	// enable ch1 only on rising edge
	TIM3->CCER = CCER_CC1S;

	// enable TIM3
	// (CR1 == control register 1)
	TIM3->CR1 |= CR1_CEN;
}
