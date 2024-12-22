#include "uart.h"
#include "stm32f4xx.h"

#define GPIOAEN     (1U << 0) // GPIOAEN is 0th bit in RCC_AHB1ENR
#define UART2EN     (1U << 17)

// baudrate info
#define SYS_FREQ		16000000 // HAL_RCC_GetHCLKFreq()
#define APB1_CLK		SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
    return ((PeriphClk + (BaudRate / 2U)) / BaudRate);
}

void uart2_rx_interrupt_init(void)
{
    // see block diagram:
    // usart2 module interfaces via RX, TX pins (which are set on GPIO pins as alternate functions)
    // usart2 module is connected to APB1 bus

    // set UART_RX pin:
    // according to datasheet, we can use PA3 or PD6 in alternate function mode for UART_RX
    // we will use PA3 in UART_RX mode
    // so gpioa must be enabled i.e have clock access
    // we will also enable interrupts on uart receive

    // configure uart:
    // enable clock access to usart2 on apb1 bus
    // set baud rate for usart2
    // configure transfer direction
    // enable the uart module via control register

    RCC->AHB1ENR |= GPIOAEN; // enable GPIOA in RCC_AHB1ENR

    // set PA3 to alternate function mode
    GPIOA->MODER |= (1U << 7);
    GPIOA->MODER &= ~(1U << 6);

    // AFRL3: bit 12 to 15
    GPIOA->AFR[0] &= ~(0xF << 12); // clear bits 12 to 15
    GPIOA->AFR[0] |= (7U << 12); // set AF07 at bits 12 to 15

    RCC->APB1ENR |= UART2EN;
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    USART2->CR1 |= UART_CR1_RE;

    // RXNEIE: RXNE interrupt enable
    USART2->CR1 |= UART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART2_IRQn);
    __enable_irq();

    // enable uart module
    USART2->CR1 |= UART_CR1_UE;
}

char uart2_read(void) {
    // UART_SR_RXNE: This bit is set by hardware when the content of the RDR shift register has been transferred
    // to the USART_DR register.
    while(!(USART2->SR & UART_SR_RXNE))
        ;

    // read the data
    return USART2->DR;
}