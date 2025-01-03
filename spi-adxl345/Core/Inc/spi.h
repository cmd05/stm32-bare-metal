#pragma once

void spi1_gpio_init(void);
void spi1_init();
void spi1_transmit(uint8_t *data, uint32_t bufsize);
void spi1_receive(uint8_t *data, uint32_t bufsize);
void spi1_cs_enable(void);
void spi1_cs_disable(void);