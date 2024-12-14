/*
 * uart.c
 *
 *  Created on: Sep 11, 2024
 *      Author: aquila
 */

#include "uart.h"

// Enable clock access
#define GPIOAEN			(1U << 0)
#define UART2EN			(1U << 17)

// UART registers
#define CR1_TE			(1U << 3)
#define CR1_RE			(1U << 2)
#define CR1_UE			(1U << 13)
#define CR1_RXNEIE		(1U << 5)

#define SYS_FREQ		16000000
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

void uart2_tx_init(void);
void uart2_write(int ch);

int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}

void uart2_tx_init(void) {
	/** configure uart GPIO pin **/
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U << 4); // set bit 4 to zero
	GPIOA->MODER |=  (1U << 5); // set bit 5 to one

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8); // clear bits at positions 8-11
	GPIOA->AFR[0] |= (7U << 8); // set bits to 7 at positions 8-11

	//	GPIOA->AFR[0] |= (1U<<8);
	//	GPIOA->AFR[0] |= (1U<<9);
	//	GPIOA->AFR[0] |= (1U<<10);
	//	GPIOA->AFR[0] &= ~(1U<<11);

	/** configure uart module **/
	// enable clock access to uart2
	RCC->APB1ENR |= UART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	// configure transfer direction
	USART2->CR1 = CR1_TE; // clear CR1 and only set CR1_TE
						  // Note: CR2 (Reset value: 0x0000 0000). So, n=1 stop bit (CR2: STOP)

	/** enable uart module **/
	USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init(void) {
	/** configure uart GPIO pin **/
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U << 4); // set bit 4 to zero
	GPIOA->MODER |=  (1U << 5); // set bit 5 to one

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8); // clear bits at positions 8-11
	GPIOA->AFR[0] |= (7U << 8); // set bits to 7 at positions 8-11

	//	GPIOA->AFR[0] |= (1U<<8);
	//	GPIOA->AFR[0] |= (1U<<9);
	//	GPIOA->AFR[0] |= (1U<<10);
	//	GPIOA->AFR[0] &= ~(1U<<11);

	// set PA3 to alternate function mode
	// set PA3 alternate function type to UART_RX (AF07)
	GPIOA->MODER &= ~(1U << 6); // set bit 6 to zero
	GPIOA->MODER |=  (1U << 7); // set bit 7 to one

	GPIOA->AFR[0] &= ~(0xF << 12); // clear bits at positions 12-15
	GPIOA->AFR[0] |= (7U << 12); // set bits to 7 at positions 12-15


	/** configure uart module **/
	// enable clock access to uart2
	RCC->APB1ENR |= UART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	// configure transfer direction
	USART2->CR1 = (CR1_TE | CR1_RE); // clear CR1 and only set CR1_TE
						  // Note: CR2 (Reset value: 0x0000 0000). So, n=1 stop bit (CR2: STOP)

	/** enable uart module **/
	USART2->CR1 |= CR1_UE;
}

void uart2_rx_interrupt_init(void) {
	/** configure uart GPIO pin **/
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U << 4); // set bit 4 to zero
	GPIOA->MODER |=  (1U << 5); // set bit 5 to one

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8); // clear bits at positions 8-11
	GPIOA->AFR[0] |= (7U << 8); // set bits to 7 at positions 8-11

	//	GPIOA->AFR[0] |= (1U<<8);
	//	GPIOA->AFR[0] |= (1U<<9);
	//	GPIOA->AFR[0] |= (1U<<10);
	//	GPIOA->AFR[0] &= ~(1U<<11);

	// set PA3 to alternate function mode
	// set PA3 alternate function type to UART_RX (AF07)
	GPIOA->MODER &= ~(1U << 6); // set bit 6 to zero
	GPIOA->MODER |=  (1U << 7); // set bit 7 to one

	GPIOA->AFR[0] &= ~(0xF << 12); // clear bits at positions 12-15
	GPIOA->AFR[0] |= (7U << 12); // set bits to 7 at positions 12-15


	/** configure uart module **/
	// enable clock access to uart2
	RCC->APB1ENR |= UART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	// configure transfer direction
	USART2->CR1 = (CR1_TE | CR1_RE); // clear CR1 and only set CR1_TE
						  // Note: CR2 (Reset value: 0x0000 0000). So, n=1 stop bit (CR2: STOP)
	// enable RXNE interrupt
	USART2->CR1 |= CR1_RXNEIE;

	// enable USART2 interrupt in NVIC
	NVIC_EnableIRQ(USART2_IRQn);
	// enable global interrupts
	__enable_irq();

	/** enable uart module **/
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void) {
	// make sure the receive data register is not empty

	while(!(USART2->SR & SR_RXNE)) // when RX is non-empty break out of the wait loop
		;

	// read data
	return USART2->DR; // uint to char conversion implicitly on return
}

void uart2_write(int ch) {
	// make sure the transmit data register is empty
	while(!(USART2->SR & SR_TXE)) // when TX is empty break out of the wait loop
		;
	// write to transmit data register
	USART2->DR = (ch & 0xFF); // keep data of rightmost 8 bits of `ch`
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
	USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
	return ((PeriphClk + (BaudRate / 2U)) / BaudRate);
}
