#include "st_bootstrap.h"

int main(void) {
	ClockInit();
	USART2_Init();
	
	GPIO_Init(GPIOA, 0, GPIO_INPUT_PULLDOWN);														// Initialize the joystick with all 4 directions and pressing it
	GPIO_Init(GPIOA, 1, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 2, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 3, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 5, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOB, 2, GPIO_OUTPUT);																		// Init red lamp on pin 2
	GPIO_Init(GPIOE, 8, GPIO_OUTPUT);

	USART2_TX_String("Welcome to embedded Systems\n");
	while (1) {
		char c = USART2_RX();
		
		if (c == 'a') GPIOB->BSRR = GPIO_BSRR_BS2;
		else if (c == 'w') GPIOB->BSRR = GPIO_BSRR_BR2;
		else if (c == 'd') GPIOE->BSRR = GPIO_BSRR_BS8;
		else if (c == 's') GPIOE->BSRR = GPIO_BSRR_BR8;
		else if (c == ' ') {
			GPIOB->BSRR = GPIO_BSRR_BR2;
			GPIOE->BSRR = GPIO_BSRR_BR8;
		} else {
			USART2_TX_String("Not able to process input...\n");
		}
		
		Delay(500);																												// make input more smooth with preventing spam
	}
}