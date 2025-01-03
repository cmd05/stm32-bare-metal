#include "stm32f4xx.h"
#include "spi.h"

// PA5 --- SPI1_SCK (AF05)
// PA6 --- SPI1_MISO (AF05)
// PA7 --- SPI1_MOSI (AF05)
// PA9 --- Slave Select (SS / NSS / CS pin)
void spi1_gpio_init(void) {
    // enable clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    // use alternate function mode for pins PA5, PA6, PA7
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    GPIOA->MODER |= GPIO_MODER_MODER6_1;
    GPIOA->MODER |= GPIO_MODER_MODER7_1;

    // set alternate function mapping to AF05 for PA5, PA6, PA7
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_2; 
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0; 
    
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_2; 
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL6_0; 
    
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL7_2; 
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL7_0;

    // set PA9 as output pin
    GPIOA->MODER |= GPIO_MODER_MODER9_0;
}

void spi1_init() {
    // note: default frequency of stm32 is 16mhz
    // without any prescalers configured the default frequency for all buses is 16mhz

    // f_pclk is frequency of bus to which the peripheral is connected (here SPI peripheral)

    // enable clock access to spi1 module
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // set clock to f_pclk/4 (ADXL345: The maximum SPI clock speed is 5 MHz)
    SPI1->CR1 |= SPI_CR1_BR_0;

    // set CPOL to 1 and CPHA to 1 (ADXL345: timing scheme follows clock polarity (CPOL) = 1 and clock phase (CPHA) = 1.)
    SPI1->CR1 |= SPI_CR1_CPOL;   
    SPI1->CR1 |= SPI_CR1_CPHA;

    // enable full duplex
    SPI1->CR1 &= ~SPI_CR1_RXONLY;

    // set MSB first
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // set mode to master
    SPI1->CR1 |= SPI_CR1_MSTR;

    // set 8 bit data mode
    SPI1->CR1 &= ~SPI_CR1_DFF;

    // select software slave management (SSM = 1)
    // set value of NSS pin to 1 (SSI = 1)
    // (SSI: This bit has an effect only when the SSM bit is set.
    // The value of this bit is forced onto the  NSS pin (here PA9) and the IO value of the NSS pin is ignored.)
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI; // disable data transfer by setting SSI bit (SS pin is active LOW)

    // enable SPI module
    SPI1->CR1 |= SPI_CR1_SPE;
}

void spi1_transmit(uint8_t *data, uint32_t bufsize) {
    uint8_t tmp;

    while(bufsize-- > 0) {
        // wait until TXE is set
        while(!(SPI1->SR & SPI_SR_TXE))
            ;

        SPI1->DR = *data++;
    }

    // for(uint32_t i = 0; i < bufsize; i++) {
    //     // wait until TXE is set
    //     while(!(SPI1->SR & SPI_SR_TXE))
    //         ;
        
    //     // write the data to the data register
    //     SPI1->DR = data[i];
    // }

    // wait until TXE is set
    while(!(SPI1->SR & SPI_SR_TXE))
        ;

    // wait for BUSY flag to reset
    while(SPI1->SR & SPI_SR_BSY)
        ;

    // clear OVR flag
    tmp = SPI1->DR;
    tmp = SPI1->SR;
}

void spi1_receive(uint8_t *data, uint32_t bufsize) {
    while(bufsize-- > 0) {
        // send dummy data
        SPI1->DR = 0;

        // wait for RXNE flag to be set
        while(!(SPI1->SR & SPI_SR_RXNE))
            ;

        // read data from register
        *data++ = SPI1->DR;
    }
}

void spi1_cs_enable(void) {
    GPIOA->ODR &= ~GPIO_ODR_ODR_9; // CS line is active LOW
}

void spi1_cs_disable(void) {
    GPIOA->ODR |= GPIO_ODR_ODR_9; // CS line is active LOW
}