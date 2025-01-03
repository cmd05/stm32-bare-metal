#include "lcd.h"
#include "systick.h"

int main() {
    lcd_init();

    while(1) {
        lcd_data('H');
        lcd_data('e');
        lcd_data('l');
        lcd_data('l');
        lcd_data('o');
        lcd_data('!');

        systickDelayMs(1000);
        lcd_command(0x1); // clear screen
    }
}