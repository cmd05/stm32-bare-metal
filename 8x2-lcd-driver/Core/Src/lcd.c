#include "stm32f4xx.h"
#include "lcd.h"
#include "systick.h"

// PC0 - PC7 = D0 to D7
// PB5       = RS
// PB6       = R/W
// PB7       = EN

#define RS      0x20
#define RW      0x40
#define EN      0x80

void gpio_lcd_init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
    GPIOB->MODER |= (GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0); // set as output pins
    GPIOB->BSRR |= (GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7); // set EN and RW pin low

    // set PC0-PC7 as output
    GPIOC->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
                        GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);
}

void lcd_init(void) {
    gpio_lcd_init();

    // init sequence:
    systickDelayMs(30);
    lcd_command(0x30);
    systickDelayMs(30);
    lcd_command(0x30);
    systickDelayMs(30);
    lcd_command(0x30);

    lcd_command(0x38); // set 8bit mode, 2 line, 5x7 font
    lcd_command(0x06); // move cursor right
    lcd_command(0x01); // clear screen, move cursor home
    lcd_command(0x0f); // turn display, blink cursor
}

void lcd_command(unsigned char cmd) {
    GPIOB->BSRR = (GPIO_BRR_BR5 | GPIO_BRR_BR6); // RS=0, R/W=0
    GPIOC->ODR = cmd;
    GPIOB->BSRR = EN;
    systickDelayMs(10);
    GPIOB->BSRR = EN << 16; // clear EN
}

void lcd_data(char data) {
    GPIOB->BSRR = RS; // RS=1
    GPIOB->BSRR = RW << 16; // RW=0

    GPIOC->ODR = data;
    GPIOB->BSRR = EN;
    systickDelayMs(10);
    GPIOB->BSRR = EN << 16; // EN=0
}