#pragma once

#include <stdint.h>

void uart2_tx_init(void);
void uart2_rxtx_init(void);
void uart2_rx_interrupt_init(void);

char uart2_read(void);

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len);