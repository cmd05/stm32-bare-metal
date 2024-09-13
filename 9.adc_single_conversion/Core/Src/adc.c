#include "stm32f4xx.h"
#include "adc.h"

// For AHB1 bus
#define GPIOAEN 		(1U << 0)
#define ADC1EN 			(1U << 8)

#define ADC_CH1			(1U << 0)
#define ADC_SEQ_LEN_1	0x00

#define CR2_AD_ON		(1U << 0)
#define CR2_SWSTART		(1U << 30)

#define SR_EOC			(1U << 1)

/**
 * Example:
 *
 * ADC configured with 3 channels.
 * ch2, ch3, ch5
 *
 * Lets say order of sampling is:
 * first = ch5
 * second = ch2
 * third = ch3
 *
 * So, put binary no. `5` in SQ1,
 * binary no. `2` in SQ2
 * binary no. `3` in SQ3
 *
 */

void pa1_adc_init(void) {
	/** configure the ADC GPIO pin **/
	// PA1 corresponds to function ADC1_IN1 (channel 1 of ADC)

	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set the mode of PA1 to analog mode
	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U << 3);

	/** configure the ADC peripheral **/

	// enable clock access to ADC
	RCC->APB2ENR |= ADC1EN; // ADC1 is connected APB2 bus

	/** configure ADC parameters **/

	// conversion sequence start
	ADC1->SQR3 = ADC_CH1; // set bits of SQ1 (in ADC_SQR3 register) as channel 1 (in binary form)

	// conversion sequence length
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	// enable the ADC module
	ADC1->CR2 |= CR2_AD_ON;
}

void start_conversion(void) {
	/** start ADC conversion **/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void) {
	// wait for conversion to be complete
	while(!(ADC1->SR & SR_EOC)) // EOC: Regular channel end of conversion
		;

	// read converted result
	return ADC1->DR;
}
