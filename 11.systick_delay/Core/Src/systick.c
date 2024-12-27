/*
 * systick.c
 *
 *  Created on: Sep 28, 2024
 *      Author: aquila
 */

#include "stm32f4xx.h"

#define SYSTICK_LOAD_VAL		16000		// (1ms) 16Mhz => 16000 cycles per ms i.e 1ms delay
#define CTRL_ENABLE				(1U << 0)
#define CTRL_CLKSRC				(1U << 2) 	// internal clock source
#define CTRL_COUNTFLAG			(1U << 16) 	// position of COUNTFLAG

// SysTick is a built-in peripheral of the ARM Cortex M4 chip.
// It contains several registers such as: STRVR, STCVR, STCSR

void systickDelayMs(int delay) { // accept `delay` in ms
	// configure systick
	// reload with number of clocks per millisecond (can be a 24 bit value)
	// setup for 1 ms delay
	// we use `SYSTICK_LOAD_VAL - 1` as we count down to zero
	SysTick->LOAD = SYSTICK_LOAD_VAL - 1; // STRVR register

	// clear systick current value register
	SysTick->VAL = 0; // STCVR register

	// enable systick and select internal clock source
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC; // STCSR register
										   	   // sets ENABLE and CLKSRC bits to 1

	//	When ENABLE is set to 1, the counter loads the RELOAD value from the SYST_RVR (LOAD) register
	//	and then counts down. On reaching 0, it sets the COUNTFLAG to 1 and optionally asserts the
	//	SysTick depending on the value of TICKINT.
	//  *It then loads the RELOAD value again, and begins counting.*

	for(int i = 0; i < delay; i++) {
		// wait until the COUNTFLAG is set (to 1)
		// COUNTFLAG: Returns 1 if timer counted to 0 since last time this was read.
		// We use COUNTFLAG to keep track if timer counted to 0 since the last time COUNTFLAG register was read

		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0) // delays for time determined by `SYSTICK_LOAD_VAL`
													 // here, we have set it for 1ms delay
			;

		// Using COUNTFLAG allows is better than checking SysTick->VAL since we may pass zero but not read that value
		// ex: 16000, 15991 ... 15, 12, 7, 4, ... (value reloaded) 16000, 15996 ...

		// while(SysTick->VAL > 10) // my hack to loop based on STCVR value
		//	;
	}

	// disable SysTick
	SysTick->CTRL = 0;
}
