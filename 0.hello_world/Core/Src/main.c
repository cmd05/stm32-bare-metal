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


int main() {
	// 1. enable clock access to GPIOA
	RCC_AHB1EN_R |= GPIOAEN; // bitwise or, set bits HIGH in RCC_AHB1EN_R only at positions in GPIOAEN where bits are HIGH

	// 2. set PA5 as output pin
	GPIOA_MODE_R |= (1U << 10); // set bit 10 to 1
	GPIOA_MODE_R &= ~(1U << 11); // set bit 11 to zero

	while(1) {
		// 3. set PA5 high
		// GPIOA_OD_R |= LED_PIN;

		// 4. experiment 2: toggle PA5
		GPIOA_OD_R ^= LED_PIN; // toggles bit as we loop (xor operation at LED_PIN bit of GPIOA_ODR)
		for(int i = 0; i < 100000; i++)
			;
	}
}
