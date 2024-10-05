#include "stm32f4xx.h"
#include "adc.h"

// For AHB1 bus
#define GPIOAEN 		(1U << 0)
#define ADC1EN 			(1U << 8)

#define ADC_CH1			(1U << 0) // set channel 1 in bits SQ1
#define ADC_CH4			(1U << 7) // set channel 4 in bits SQ2

#define ADC_SEQ_LEN_1	(0x00) // 1 conversion
#define ADC_SEQ_LEN_2	(1U << 20) // 2 conversions (0001) - set bit 20

#define CR2_AD_ON		(1U << 0)
#define CR2_CONT		(1U << 1)

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

	// enable clock access to ADC (clock access must be enabled before setting channels in SQR registers)
	RCC->APB2ENR |= ADC1EN; // ADC1 is connected APB2 bus

	// set the mode of PA1 to analog mode
	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U << 3);

	ADC1->SQR3 |= ADC_CH1; // set bits of SQ1 (in ADC_SQR3 register) as channel 1 (in binary form)

	// set the mode of PA4 to analog mode
	GPIOA->MODER |= (1U << 8);
	GPIOA->MODER |= (1U << 9);

	ADC1->SQR3 |= ADC_CH4; // set bits of SQ2 (in ADC_SQR3 register) as channel 4 (in binary form)

	/** configure ADC parameters **/

	// conversion sequence length
	ADC1->SQR1 |= ADC_SEQ_LEN_2;

	// enable the ADC module
	ADC1->CR2 |= CR2_AD_ON;
}

void pa2_adc_init(void) {
//	/** configure the ADC GPIO pin **/
//	// PA2 corresponds to function ADC1_IN2 [Table 8. STM32F401xD/xE pin definitions] (channel 2 of ADC)
//
//	// enable clock access to GPIOA
//	RCC->AHB1ENR |= GPIOAEN;
//
//	// set the mode of PA2 to analog mode
//	GPIOA->MODER |= (1U << 4);
//	GPIOA->MODER |= (1U << 5);
//
//	/** configure the ADC peripheral **/
//
//	// enable clock access to ADC
//	RCC->APB2ENR |= ADC1EN; // ADC1 is connected APB2 bus
//
//	/** configure ADC parameters **/
//
//	// conversion sequence start
//	ADC1->SQR3 |= ADC_CH4; // set bits of SQ2 (in ADC_SQR3 register) as channel 2 (in binary form)
//
//	// conversion sequence length
//	ADC1->SQR1 |= ADC_SEQ_LEN_2;
//
//	// enable the ADC module
//	ADC1->CR2 |= CR2_AD_ON;
}

void start_conversion(void) {
	// enable continuous conversion
//	ADC1->CR2 |= CR2_CONT;

	// EOCS=1: The EOC bit is set at the end of each regular conversion.
//	ADC1->CR2 |= (1U << 10);

	// enable discontinous mode - 2 channels
	ADC1->CR1 |= (1U << 11); // enable discontinuous mode
	ADC1->CR1 |= (1U << 13); // set as 2 channels

	/** start ADC conversion **/
	ADC1->CR2 |= CR2_SWSTART;
}

uint32_t adc_read(void) {
	// wait for conversion to be complete
	while(!(ADC1->SR & SR_EOC)) // EOC: Regular channel end of conversion
		;

	// for multi channel conversion
	// 1. use dma
	// 2. use eocie bit and cr1->scan bit for interrupts
	// 3. use EOCS=1 and discontinuous mode (https://community.st.com/t5/stm32-mcus-products/adc-s-eoc-event/td-p/519680)

	// read converted result
	return ADC1->DR;
}
