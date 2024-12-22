#include "stm32f4xx.h"
#include <stdint.h>
#include "adc.h"

#define GPIOAEN     (1U << 0)
#define ADC1EN      (1U << 8)

#define ADC_CH1         (1U << 0)
#define ADC_SEQ_LEN_1   0x00

#define ADC_CR2_AD_ON       (1U << 0)
#define ADC_CR2_SWSTART     (1U << 30)
#define ADC_CR2_CONT        (1U << 1)

#define ADC_CR1_EOCIE   (1U << 5)

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

void pa1_adc_interrupt_init(void) {
    // configure the ADC GPIO pin
    // PA1 corresponds to function ADC1_IN1 (channel 1 of ADC)
    
    // enable clock access to GPIOA
    RCC->AHB1ENR |= GPIOAEN;

    // set the mode of PA1 to analog mode
    GPIOA->MODER |= ((1U << 2) | (1U << 3));

    // configure the adc peripheral

    // enable clock access to adc
    RCC->APB2ENR |= ADC1EN;

    // enable EOC interrupt
    ADC1->CR1 |= ADC_CR1_EOCIE;
    NVIC_EnableIRQ(ADC_IRQn);
    __enable_irq();

    // configure the adc parameters

    // conversion sequence start
    ADC1->SQR3 = ADC_CH1;

    //  conversion sequence length
    ADC1->SQR1 = ADC_SEQ_LEN_1;

    // enable the adc module
    ADC1->CR2 |= ADC_CR2_AD_ON;
}

void start_conversion(void) {
    // enable the continuous conversion
    ADC1->CR2 |= ADC_CR2_CONT;

    // start the adc conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adc_read(void) {
    // wait for conversion to be complete
    while(!(ADC1->SR & ADC_SR_EOC))
        ;

    return ADC1->DR;
}