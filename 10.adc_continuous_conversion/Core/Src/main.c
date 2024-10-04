#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

// Single channel - single conversion mode
int main() {
	uart2_tx_init(); // write data to serial
	pa1_adc_init();

	// STM32F4 has 12 bit ADC
	// At GND: adc_read gives approximately 0
	// At 3.3V: adc_read gives approximately 4095 // largest 12 bit value

	start_conversion(); // start conversion in continuous conversion mode

	while(1) {
		sensor_value = adc_read();
		printf("Sensor value: %lu\n\r", sensor_value); // sent from board to computer
	}
}
