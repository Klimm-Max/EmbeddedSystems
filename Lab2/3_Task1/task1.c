#include "st_bootstrap.h"

int main(void) {
	ClockInit();
	USART2_Init();
	SysTickInit();
	/*
	GPIO_Init(GPIOA, 0, GPIO_INPUT_PULLDOWN);														// Initialize the joystick with all 4 directions and pressing it
	GPIO_Init(GPIOA, 1, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 2, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 3, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 5, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOB, 2, GPIO_OUTPUT);																		// Init red lamp on pin 2
	GPIO_Init(GPIOE, 8, GPIO_OUTPUT);
	*/
	USART2_TX_String("Welcome to embedded Systems\n");
	
	while (1) {
		USART2_TX(USART2_RX());
	}
}