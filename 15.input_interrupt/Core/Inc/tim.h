/*
 * tim.h
 *
 *  Created on: Sep 29, 2024
 *      Author: aquila
 */

#ifndef INC_TIM_H_
#define INC_TIM_H_

void tim2_pa5_output_compare(void);
void tim3_pa6_input_capture(void);

#define SR_UIF		(1U << 0)
#define SR_CC1IF	(1U << 1)

#endif /* INC_TIM_H_ */
