#include "stm32f4xx.h"
#include "i2c.h"
#include "uart.h"
#include "systick.h"

#define I2C_100KHZ               80
#define SD_MODE_MAX_RISE_TIME    17

void I2C1_scan_bus(void) {
    int a = 0;
    for (uint8_t i = 0; i < 128; i++) {
        I2C1->CR1 |= I2C_CR1_START;
        while (!(I2C1->SR1 & I2C_SR1_SB));
        I2C1->DR = (i << 1 | 0);
        while (!(I2C1->SR1) | !(I2C1->SR2)) {};
        I2C1->CR1 |= I2C_CR1_STOP;
        systickDelayMs(10); //minium wait time is 40 uS, but for sure, leave it 100 uS
        a = (I2C1->SR1 & I2C_SR1_ADDR);
        if (a == 2) {
            printf("Found I2C device at address 0x%X (hexadecimal), or %d (decimal)\n\r", i, i);
        }
    }
}

void I2C1_init(void) {
    // PB8 --- SCL (AF04)
    // PB9 --- SDA (AF04)

    // enable clock access to GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    // set PB8 and PB9 mode to alternate function
    GPIOB->MODER |= GPIO_MODER_MODE8_1;
    GPIOB->MODER &= ~GPIO_MODER_MODE8_0;
    
    GPIOB->MODER |= GPIO_MODER_MODE9_1;
    GPIOB->MODER &= ~GPIO_MODER_MODE9_0;

    // set to AF04
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;

    // set PB8 and PB9 output type to open drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT8;
    GPIOB->OTYPER |= GPIO_OTYPER_OT9;

    // enable pull-up for PB8 and PB9
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD8_1;
    
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0;
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD9_1;

    // enable clock access to I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // enter and exit I2C software reset mode
    I2C2->CR1 = I2C_CR1_SWRST;
    I2C2->CR1 &= ~I2C_CR1_SWRST;

    // set frequency to 16Mhz
    I2C1->CR2 |= I2C_CR2_FREQ_4;

    // disable I2C module
    I2C1->CR1 &= ~I2C_CR1_PE;

    // set peripheral clock frequency
    // by default we are using Sm Mode
    // CCR= T_high / T_pclk1 
    // T_high = 1 / (2 * 100KHz) = 5us = 5000ns
    // Assume T_PCLK1 is 16MHz (equal to system clock) (APB1 bus)
        // 1 / 16Mhz = 62.5ns
    // So, CCR = 5000 / 62.5ns = 80
    I2C1->CCR = I2C_100KHZ;

    // set rise time
    // If, in the I2C_CR2 register, the value of FREQ[5:0] bits is equal to 0x08 and T_PCLK1 = 125 ns 
    // therefore the TRISE[5:0] bits must be programmed with 09h.
    // (1000 ns / 125 ns = 8 => 8 + 1 = 9)
    // So for T_PCLK1 = 62.5ns => 1000/62.5 = 16 => 16 + 1 = 17
    I2C1->TRISE = SD_MODE_MAX_RISE_TIME;

    // enable I2C module
    I2C1->CR1 |= I2C_CR1_PE;
}

// Master - STM32
// Slave - Device such as ADXL345
void I2C1_byteRead(char saddr, char maddr, char *data) {
    volatile int tmp;

    // wait until bus is not busy
    while(I2C1->SR2 & I2C_SR2_BUSY)
        ;

    // generate START condition
    // Master mode is selected as soon as the Start condition is generated on the bus with a 
    // START bit
    I2C1->CR1 |= I2C_CR1_START;

    // wait until START flag is set
    while(!(I2C1->SR1 & I2C_SR1_SB))
        ;

    
    // The master can decide to enter Transmitter or Receiver mode depending on the LSB of the 
    // slave address sent.
    // To enter Transmitter mode, a master sends the slave address with LSB reset (0).
    
    // write slave address to DR register and enter write mode (R/W i.e LSB bit is not set)
    // i.e I2C_DR: LSB is R/W and bits 1-7 contain slave address
    I2C1->DR = (saddr << 1) | 0;

    while(!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    // clear ADDR by read
    tmp = I2C1->SR2;

    // transmit memory address inside slave
    I2C1->DR = maddr;

    // wait until transmitter is empty
    while(!(I2C1->SR1 & I2C_SR1_TXE))
        ;

    // RECEIVE BYTE

    // enable acknowledge
    I2C1->CR1 |= I2C_CR1_ACK;

    // Generate Restart
    I2C1->CR1 |= I2C_CR1_START;
    
    // wait until START flag is set
    while(!(I2C1->SR1 & I2C_SR1_SB))
        ;


    // enter receiver mode (read from slave)
    // (R/W bit is set)
    I2C1->DR = (saddr << 1) | 1;

    // wait until addr flag is set
    while(!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    // clear ADDR by read
    tmp = I2C1->SR2;

    // disable acknowledge
    I2C1->CR1 &= ~I2C_CR1_ACK;

    // generate STOP after data received
    I2C1->CR1 |= I2C_CR1_STOP;

    systickDelayMs(10); //minium wait time is 40 uS, but for sure, leave it 100 uS

    // wait until RXNE flag is set
    while(!(I2C1->SR1 & I2C_SR1_RXNE))
        ;
    
    // I2C_DIR: – Receiver mode: Received byte is copied into DR (RxNE=1). A continuous transmit stream
    // can be maintained if DR is read before the next data byte is received (RxNE=1).
    *data = I2C1->DR; // read into data then increment pointer
}

void I2C1_burstRead(char saddr, char maddr, int n, char* data) {
    volatile int tmp;

    // wait until bus is not busy
    while(!(I2C1->SR2 & I2C_SR2_BUSY))
        ;
    
    // generate START
    I2C1->CR1 |= I2C_CR1_START;

    // wait until START flag is set
    while(!(I2C1->CR1 & I2C_CR1_START))
        ;

    // master transmitter mode
    // transmit slave address + write
    I2C1->DR = saddr << 1;

    // wait until ADDR flag is set
    while(!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    // clear ADDR flag
    tmp = I2C1->SR2;

    // wait until transmitter is empty
    while(!(I2C1->SR1 & I2C_SR1_TXE))
        ;

    // send memory address
    I2C1->DR = maddr;

    // wait until transmitter is empty
    while(!(I2C1->SR1 & I2C_SR1_TXE))
        ;
    
    // generate Restart
    I2C1->CR1 |= I2C_CR1_START;

    // wait until START flag is set
    while(!(I2C1->CR1 & I2C_CR1_START))
        ;

    // transmit slave address + read
    I2C1->DR = (saddr << 1) | 1;

    // wait until ADDR flag is set
    while(!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    // clear ADDR flag
    tmp = I2C1->SR2;

    // enable acknowledge
    I2C1->CR1 |= I2C_CR1_ACK;

    // read in burst mode
    // In a burst transfer, the address for write or read transfer is just an incremental value of previous address. 
    // Hence in a 4-beat incremental burst transfer (write or read), if the starting address is 'A', then the consecutive addresses will be 'A+m', 'A+2*m', 'A+3*m'.
    // Hence, in burst read/write, `maddr` will be automatically incremented by the slave by 1 byte (i.e to start of next register)

    // After each byte the interface generates in sequence:
    // 1. An acknowledge pulse if the ACK bit is set
    // 2. The RxNE bit is set 
    while(n > 0) {
        if(n == 1) {
            // disable acknowledge
            I2C1->CR1 &= ~I2C_CR1_ACK;

            // generate STOP
            I2C1->CR1 |= I2C_CR1_STOP;

            // wait until RXNE flag is set
            while(!(I2C1->SR1 & I2C_SR1_RXNE))
                ;

            // read data from DR
            *data++ = I2C1->DR;

            break;
        } else {
            // wait until RXNE flag is set
            while(!(I2C1->SR1 & I2C_SR1_RXNE))
                ;
            
            // read data from DR
            *data++ = I2C1->DR;

            n--;
        }
    }
}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data) {
    volatile int tmp;

    // wait until bus is not busy
    while(!(I2C1->SR2 & I2C_SR2_BUSY))
        ;

    // generate START
    I2C1->CR1 |= I2C_CR1_START;

    // wait until START is set
    while(!(I2C1->CR1 & I2C_CR1_START))
        ;

    // transmit slave address + write
    I2C1->DR = saddr << 1;

    // wait until ADDR flag is set
    while(!(I2C1->SR1 & I2C_SR1_ADDR))
        ;
    // clear ADDR flag
    tmp = I2C1->SR2;

    // wait until data register is empty
    while(!(I2C1->SR1 & I2C_SR1_TXE))
        ;
    
    // send memory address
    I2C1->DR = maddr;
    
    for(int i = 0; i < n; i++) {
        // When the acknowledge pulse is received, the TxE bit is set by hardware
        // wait until data register is empty
        while(!(I2C1->SR1 & I2C_SR1_TXE))
            ;
        
        // transmit memory address
        I2C1->DR = *data++;
    }

    // BTF: – In transmission when a new byte should be sent and DR has not been written yet (TxE=1).
    // wait until transfer is finished
    while(!(I2C1->SR1 & I2C_SR1_BTF))
        ;

    // generate STOP
    I2C1->CR1 |= I2C_CR1_STOP;
}