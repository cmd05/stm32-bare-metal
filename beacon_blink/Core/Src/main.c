// LED
// Port: A
// Pin: 5

// buses carry clock to various parts of the microcontroller
// Advanced Peripheral Bus (APB)
// Advanced High-performance Bus (AHB)

// APB is slower than AHB

// *peripherals* have a number of *registers*
// registers are a set of bits

#define PERIPH_BASE 		0x40000000UL
#define AHB1PERIPH_OFFSET	0x00020000UL
#define AHB1PERIPH_BASE		(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET		0x00UL // GPIOA starts at AHB1PERIPH_BASE

#define GPIOA_BASE			(AHB1PERIPH_BASE + GPIOA_OFFSET)

#define RCC_OFFSET			0x3800
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET) // RCC: Reset and Clock Control

#define AHB1EN_R_OFFSET		0x30
#define RCC_AHB1EN_R		(*(volatile unsigned int *) (RCC_BASE + AHB1EN_R_OFFSET)) // enable register for RCC

// direction register: set pin to either input or output
// data register: for input or output

#define MODE_R_OFFSET		0x00UL
#define GPIOA_MODE_R		(*(volatile unsigned int *) (GPIOA_BASE + MODE_R_OFFSET)) // cast to pointer and then dereference it

// bitshifting: 1U << 4 // shift `1` to position 4

// (RCC_AHB1ENR):
// 		Bit 0 GPIOAEN: IO port A clock enable
#define GPIOAEN				(1U << 0) // shift `1` to position zero

// 1U << 10 // set bit 10 to 1
// &= ~(1U << 11) // set (only) bit 11 to zero

#define OD_R_OFFSET			0x14UL
#define GPIOA_OD_R			(*(volatile unsigned int *) (GPIOA_BASE + OD_R_OFFSET))

#define PIN5				(1U << 5)
#define LED_PIN				PIN5

int state = 0;

void blink() {
	GPIOA_OD_R ^= LED_PIN; // toggles bit as we loop (xor operation at LED_PIN bit of GPIOA_ODR)
	state = !state;
}

void delay_num(int d) {
    // unmodifiable compiler code
	// volatile int i = 0;
	for(int i = 0; i < d; i++) __asm("");

}

// void delay(int n) {
// 	for(int i = 0; i < n; i++);
// }

void inline __attribute__((always_inline)) delay(int delay)
{
   while(delay--) __asm("");
}

int main() {
int NUM_DELAY = 100000;
	
  // 1. enable clock access to GPIOA
	RCC_AHB1EN_R |= GPIOAEN; // bitwise or, set bits HIGH in RCC_AHB1EN_R only at positions in GPIOAEN where bits are HIGH

	// 2. set PA5 as output pin
	GPIOA_MODE_R |= (1U << 10); // set bit 10 to 1
	GPIOA_MODE_R &= ~(1U << 11); // set bit 11 to zero

	GPIOA_OD_R |= (0u << LED_PIN); // initially off

	while(1) {
		blink();
		// for(int i = 0; i < NUM_DELAY; i++);
		delay_num(NUM_DELAY * 10);
		blink();
		// for(int i = 0; i < NUM_DELAY * 10; i++);
		delay_num(NUM_DELAY * 40);
	}
}