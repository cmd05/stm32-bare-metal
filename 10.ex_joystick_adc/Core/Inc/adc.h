/*
 * adc.h
 *
 *  Created on: Sep 12, 2024
 *      Author: aquila
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

#define CR2_SWSTART		(1U << 30)

void pa1_adc_init(void);
void pa2_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* INC_ADC_H_ */
