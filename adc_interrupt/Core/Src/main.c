#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main() {
    uart2_tx_init(); // write data to serial
    pa1_adc_interrupt_init();

    // STM32F4 has 12 bit ADC
	// At GND: adc_read gives approximately 0
	// At 3.3V: adc_read gives approximately 4095 // largest 12 bit value

    start_conversion();

    while(1) {
    
    }
}

static void adc_callback(void) {
    sensor_value = ADC1->DR;
    printf("Sensor value: %lu\n\r", sensor_value);
}

void ADC_IRQHandler(void) {
    // EOC: Regular channel end of conversion
    // This bit is set by hardware at the end of the conversion of a regular group of channels. It is
    // cleared by software or by reading the ADC_DR register.
    // 0: Conversion not complete (EOCS=0), or sequence of conversions not complete (EOCS=1)
    // 1: Conversion complete (EOCS=0), or sequence of conversions complete (EOCS=1)
    if((ADC1->SR & ADC_SR_EOC) != 0) { // EOC interrupt
        // ADC1->SR &= ~ADC_SR_EOC;

        adc_callback();
    }
}