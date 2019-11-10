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
		if (GPIOA->IDR & GPIO_IDR_ID1) USART2_TX('L');										// If GPIOA data register is set and data register on Pin 1 is set -> send character to USART
		else if (GPIOA->IDR & GPIO_IDR_ID2) USART2_TX('R');
		else if (GPIOA->IDR & GPIO_IDR_ID3) USART2_TX('U');
		else if (GPIOA->IDR & GPIO_IDR_ID5) USART2_TX('D');
		else if (GPIOA->IDR & GPIO_IDR_ID0) USART2_TX('S');
		Delay(500);
	}
}