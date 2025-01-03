#pragma once

void gpio_lcd_init(void);
void lcd_init(void);
void lcd_command(unsigned char cmd);
void lcd_data(char data);