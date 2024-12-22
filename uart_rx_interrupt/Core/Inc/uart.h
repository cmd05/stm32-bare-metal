// uart info
// (or we can directly use stm32f401xe.h)

#define UART_CR1_RE         (1U << 2)
#define UART_CR1_RXNEIE     (1U << 5)
#define UART_CR1_UE         (1U << 13)
#define UART_SR_RXNE        (1U << 5)

void uart2_rx_interrupt_init(void);