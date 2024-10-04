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
	tim2_pa5_output_compare();

	while(1) {

	}
}
