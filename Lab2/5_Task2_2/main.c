#include "st_bootstrap.h"
#include <string.h>

char buffer[7];
int currentIndex = 0;

void processBuffer(char input);
void resetBuffer(void);

int main(void) {
	ClockInit();
	USART2_Init();
	GPIO_Init(GPIOB, 2, GPIO_OUTPUT);																		// Init red lamp on pin 2 of GPIOB
	GPIO_Init(GPIOE, 8, GPIO_OUTPUT);																		// Init green lamp on pin 8 of GPIOE

	USART2_TX_String("Welcome to embedded Systems\n");
	while (1) {
		char c = USART2_RX();	
		processBuffer(c);
		Delay(50);																												// make input more smooth with preventing "spam"
	}
}

void processBuffer(char input) {
	if (input == '\r') {																								// if enter was pressed by user
		 if (!(strstr("right", buffer) == NULL)) {
			 GPIOE->BSRR = GPIO_BSRR_BS8;
			 USART2_TX_String("\nturned on the green lamp!\n");
		 }
		 else if (!(strstr("up", buffer) == NULL)) {
			 GPIOB->BSRR = GPIO_BSRR_BR2;
			 USART2_TX_String("\nturned off the red lamp!\n");
		 }
		 else if (!(strstr("down", buffer) == NULL)) {
			 GPIOE->BSRR = GPIO_BSRR_BR8;
			 USART2_TX_String("\nturned off the green lamp!\n");
		 }
		 else if (!(strstr("left", buffer) == NULL)) {
			 GPIOB->BSRR = GPIO_BSRR_BS2;
			 USART2_TX_String("\nturned on the red lamp!\n");
		 }
		 else if (!(strstr("disable", buffer) == NULL)) {
			GPIOB->BSRR = GPIO_BSRR_BR2;
			GPIOE->BSRR = GPIO_BSRR_BR8;
			USART2_TX_String("\ndisabled both lamps!\n");
		 } else {
			USART2_TX_String("\nUnable to process input!\n");
		 }
		 
			resetBuffer();
	} else if (input == '\b') {																					// if backspace was pressed in order to delte symbols
		if (currentIndex > 0) {
			buffer[currentIndex] = NULL;
			currentIndex--;
		}
	} else {
		if (currentIndex <= 6) {
			buffer[currentIndex] = input;
			USART2_TX(input);
			currentIndex++;
		}
	}
}

void resetBuffer(void) {
	currentIndex = 0;
	for (int i=0; i < 7; i++) {
		buffer[i] = NULL;
		USART2_TX(buffer[i]);
	}
}