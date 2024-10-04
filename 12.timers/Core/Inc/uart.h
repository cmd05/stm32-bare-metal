/*
 * uart.h
 *
 *  Created on: Sep 11, 2024
 *      Author: aquila
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

void uart2_tx_init(void);
void uart2_rxtx_init(void);
char uart2_read(void);

#endif /* INC_UART_H_ */
