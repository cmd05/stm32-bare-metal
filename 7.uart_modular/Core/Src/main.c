#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>

#include "uart.h"

int main() {
	uart2_tx_init();

	while(1) {
		printf("Hello from STM32 F4 :D ...\n\r");
	}
}




