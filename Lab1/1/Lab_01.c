#include <stm32l4xx.h>

int main(void) {
	RCC->AHB2ENR = RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;			// enable GPIO Port B and E in general
	GPIOB->MODER &= ~GPIO_MODER_MODE2_1;													// initialize the adress space of Port B thus using Pin 2
	GPIOE->MODER &= ~GPIO_MODER_MODE8_1;													// initialize the adress space of Port E thus using Pin 8

	while(1) {
		GPIOB->BSRR = GPIO_BSRR_BS2;																// BitSetResetRegister of Port B will set Pin 2 to 1 (equal to a Bit sequence but more readable)
		GPIOE->BSRR = GPIO_BSRR_BR8;
		
		
		for (int i=0; i < 1000000; i++);
		
		GPIOB->BSRR = GPIO_BSRR_BR2;																// BitSetResetRegister of Port B will set Pin 2 to 0 (equal to a Bit sequence but more readable)
		GPIOE->BSRR = GPIO_BSRR_BS8;
		
		for (int i=0; i < 1000000; i++);
	}
}