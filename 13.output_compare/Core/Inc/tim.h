/*
 * tim.h
 *
 *  Created on: Sep 29, 2024
 *      Author: aquila
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

void tim2_1hz_init(void);
void tim2_pa5_output_compare(void);

#define SR_UIF		(1U << 0)

#endif /* INC_TIM_H_ */
