#include <stdint.h>

// LED
// Port: A
// Pin: 5

//#define __IO volatile


#define PERIPH_BASE 		0x40000000UL
#define AHB1PERIPH_OFFSET	0x00020000UL
#define AHB1PERIPH_BASE		PERIPH_BASE + AHB1PERIPH_OFFSET
#define GPIOA_OFFSET		0x00UL // GPIO starts at AHB1PERIPH_BASE

#define GPIOA_BASE			AHB1PERIPH_BASE + GPIOA_OFFSET

#define RCC_OFFSET			0x3800
#define RCC_BASE			AHB1PERIPH_BASE + RCC_OFFSET // RCC: Reset and Clock Control

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

#define PIN5				1U << 5
#define LED_PIN				PIN5

// each register in GPIO is 4 bytes
// order of registers must be in the same order as the offsets in the datasheet
// starting address of first member of struct is same as address of struct (i.e MODER register and GPIOx will have same address)
//typedef struct {
//	__IO uint32_t MODER;		// GPIO port mode register							Address offset: 0x00
//	__IO uint32_t OTYPER; 		// GPIO port output type register					Address offset: 0x04
//	__IO uint32_t OSPEEDR;		// GPIO port output speed register					Address offset: 0x08
//	__IO uint32_t PUPDR;		// GPIO port pull-up/pull-down register 			Address offset: 0x0C
//	__IO uint32_t IDR;			// GPIO port input data register					Address offset: 0x10
//	__IO uint32_t ODR;			// GPIO port output data register					Address offset: 0x14
//	__IO uint32_t BSRR;			// GPIO port bit set/reset register					Address offset: 0x18
//	__IO uint32_t LCKR;			// GPIO port configuration lock register			Address offset: 0x1C
//	__IO uint32_t AFR[2];		// GPIO alternate function registers (low,high)		Address offset: 0x20-0x24
//} GPIO_Typedef;

// GPIO peripheral
typedef struct {
	volatile uint32_t MODER;		// GPIO port mode register							Address offset: 0x00
	volatile uint32_t DUMMY[4];		// spacing between MODER and ODR
	volatile uint32_t ODR;			// GPIO port output data register					Address offset: 0x14
} GPIO_TypeDef;

// RCC Peripheral
//typedef struct
//{
//  __IO uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
//  __IO uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
//  __IO uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
//  __IO uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
//  __IO uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
//  __IO uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
//  __IO uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
//  uint32_t      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
//  __IO uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
//  __IO uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
//  uint32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
//  __IO uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
//  __IO uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
//  __IO uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
//  uint32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
//  __IO uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
//  __IO uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
//  uint32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
//  __IO uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
//  __IO uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
//  __IO uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
//  uint32_t      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
//  __IO uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
//  __IO uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
//  uint32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
//  __IO uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
//  __IO uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
//  uint32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
//  __IO uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
//  __IO uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
//  uint32_t      RESERVED7[1];  /*!< Reserved, 0x88                                                                    */
//  __IO uint32_t DCKCFGR;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */
//} RCC_TypeDef;

// RCC Peripheral
typedef struct
{
  volatile uint32_t DUMMY[8];
  volatile uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register		Address offset: 0x30 */
} RCC_TypeDef;

// *note*: we are not creating instances of structs in the code
// we are simply casting memory addresses to our peripheral structs
// and accessing the registers through the interface of struct members
#define RCC			((RCC_TypeDef *) RCC_BASE)
#define GPIOA		((GPIO_TypeDef *) GPIOA_BASE)

int main() {
	// 1. enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// 2. set PA5 as output pin
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	while(1) {
		// 3. toggle led
		GPIOA->ODR ^= LED_PIN;
		for(int i = 0; i < 100000; i++)
			;
	}
}
