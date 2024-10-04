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
int timestamp = 0;

/* Setup: Connect a jumper wire from PA5 to PA6 */
// Timer at PA6 uses PA5 as the source for timestamps
// PA5 (inbuilt LED) is being blinked every second by tim2 using output capture mode.
// So we get a square wave at PA5 which is the input for PA6 using a jumper wire.
int main() {
	tim2_pa5_output_compare();
	tim3_pa6_input_capture();

	while(1) {
		// wait until edge is captured
		while((TIM3->SR & SR_CC1IF) == 0) {}
		// read captured value
		timestamp = TIM3->CCR1;
	}
}
