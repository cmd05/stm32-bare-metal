#include "stm32f4xx.h"
#include "uart.h"

// baudrate info
#define SYS_FREQ		16000000 // HAL_RCC_GetHCLKFreq()
#define APB1_CLK		SYS_FREQ
#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);
static void uart2_write(int ch);

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate) {
    USARTx->BRR = compute_uart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate) {
    return ((PeriphClk + (BaudRate / 2U)) / BaudRate);
}

int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}


void uart2_tx_init(void) {
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U << 4); // set bit 4 to zero
	GPIOA->MODER |=  (1U << 5); // set bit 5 to one

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8); // clear bits at positions 8-11
	GPIOA->AFR[0] |= (7U << 8); // set bits to 7 at positions 8-11

	// enable clock access to uart2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	// configure transfer direction
	USART2->CR1 = USART_CR1_TE; // clear CR1 and only set CR1_TE
						        // Note: CR2 (Reset value: 0x0000 0000). So, n=1 stop bit (CR2: STOP)

	/** enable uart module **/
	USART2->CR1 |= USART_CR1_UE;
}

void uart2_rx_interrupt_init(void)
{
    // see block diagram:
    // usart2 module interfaces via RX, TX pins (which are set on GPIO pins as alternate functions)
    // usart2 module is connected to APB1 bus

    // set UART_RX pin:
    // according to datasheet, we can use GPIO pins PA3 or PD6 in alternate function mode for UART_RX
    // we will use GPIO Pin PA3 in UART_RX mode
    // so GPIOA must be enabled i.e have clock access
    // we will also enable interrupts on uart receive

    // configure uart:
    // enable clock access to usart2 on apb1 bus
    // set baud rate for usart2
    // configure transfer direction
    // enable the uart module via control register

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable GPIOA in RCC_AHB1ENR

    // set PA3 to alternate function mode
    GPIOA->MODER |= (1U << 7);
    GPIOA->MODER &= ~(1U << 6);

    // AFRL3: bit 12 to 15
    GPIOA->AFR[0] &= ~(0xF << 12); // clear bits 12 to 15
    GPIOA->AFR[0] |= (7U << 12); // set AF07 at bits 12 to 15

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    USART2->CR1 |= USART_CR1_RE;

    // RXNEIE: RXNE interrupt enable
    USART2->CR1 |= USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART2_IRQn);
    __enable_irq();

    // enable uart module
    USART2->CR1 |= USART_CR1_UE;
}

void uart2_rxtx_init(void) {
	/** configure uart GPIO pin **/
	// enable clock access to GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// set PA2 mode to alternate function mode
	GPIOA->MODER &= ~(1U << 4); // set bit 4 to zero
	GPIOA->MODER |=  (1U << 5); // set bit 5 to one

	// set PA2 alternate function type to UART_TX (AF07)
	GPIOA->AFR[0] &= ~(0xF << 8); // clear bits at positions 8-11
	GPIOA->AFR[0] |= (7U << 8); // set bits to 7 at positions 8-11

	// set PA3 to alternate function mode
	// set PA3 alternate function type to UART_RX (AF07)
	GPIOA->MODER &= ~(1U << 6); // set bit 6 to zero
	GPIOA->MODER |=  (1U << 7); // set bit 7 to one

	GPIOA->AFR[0] &= ~(0xF << 12); // clear bits at positions 12-15
	GPIOA->AFR[0] |= (7U << 12); // set bits to 7 at positions 12-15


	/** configure uart module **/
	// enable clock access to uart2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// configure baudrate
	uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

	// configure transfer direction
	USART2->CR1 = (USART_CR1_TE | USART_CR1_RE); // clear CR1 and only set CR1_TE
						  // Note: CR2 (Reset value: 0x0000 0000). So, n=1 stop bit (CR2: STOP)

	/** enable uart module **/
	USART2->CR1 |= USART_CR1_UE;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len) {
    // USART2_TX is on DMA1,Stream6-CH4
    // memory to peripheral (USART2) transfer direction
    
    // enable clock access to DMA
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

    // disable DMA1 stream 6
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;

    // wait until DMA1 Stream6 is disabled
    while((DMA1_Stream6->CR & DMA_SxCR_EN) != 0)
        ;

    // clear all interrupt flags of stream 6
    // Note: setting the bit (i.e to 1) in DMA_HIFCR will clear the corresponding interrupt flag in DMA_HISR
        // Bits 27, 21, 11, 5 CTCIFx: Stream x clear transfer complete interrupt flag (x = 7..4)
        // Writing 1 to this bit clears the corresponding TCIFx flag in the DMA_HISR register
    DMA1->HIFCR |= (DMA_HIFCR_CFEIF6 | DMA_HIFCR_CDMEIF6 | DMA_HIFCR_CTEIF6 | DMA_HIFCR_CHTIF6 | DMA_HIFCR_CTCIF6);

    // set the destination buffer
    DMA1_Stream6->PAR = dst; // DMA stream x peripheral address register (since destination is USART2 peripheral)

    // set the source buffer
    DMA1_Stream6->M0AR = src;

    // set length
    // DMA_SxNDTR: Number of data items to be transferred (0 up to 65535). This register can be written only 
    // when the stream is disabled. When the stream is enabled, this register is read-only, 
    // indicating the remaining data items to be transmitted. This register decrements after each 
    // DMA transfer.
    DMA1_Stream6->NDTR = len;

    // select stream6 CH4
    DMA1_Stream6->CR |= DMA_SxCR_CHSEL_2;

    // enable memory increment
    DMA1_Stream6->CR |= DMA_SxCR_MINC;

    // configure transfer direction
    DMA1_Stream6->CR |= DMA_SxCR_DIR_0; // 01: Memory-to-peripheral

    // enable DMA transfer complete interrupt
    DMA1_Stream6->CR |= DMA_SxCR_TCIE;

    // enable direct mode and disable FIFO
    // DMA stream x FIFO control register (DMA_SxFCR)
    // DMDIS: 
        // 0: Direct mode enabled
    DMA1_Stream6->FCR &= ~DMA_SxFCR_DMDIS; // (check if this errors while testing then disable all bits by setting register to zero)

    // enable stream 6
    DMA1_Stream6->CR |= DMA_SxCR_EN;

    // enable UART2 transmitter DMA
    USART2->CR3 |= USART_CR3_DMAT;

    // enable DMA interrupt in NVIC
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

char uart2_read(void) {
    // UART_SR_RXNE: This bit is set by hardware when the content of the RDR shift register has been transferred
    // to the USART_DR register.
    while(!(USART2->SR & USART_SR_RXNE))
        ;

    // read the data
    return USART2->DR;
}

static void uart2_write(int ch) {
	// make sure the transmit data register is empty
	while(!(USART2->SR & USART_SR_TXE)) // when TX is empty break out of the wait loop
		;
	// write to transmit data register
	USART2->DR = (ch & 0xFF); // keep data of rightmost 8 bits of `ch`
}