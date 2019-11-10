#ifndef _ST_BOOTSTRAP_
#define _ST_BOOTSTRAP_

#ifndef STM32L476XX
#define STM32L476XX
#endif
#include <stm32l4xx.h>

typedef enum GPIO_Mode { GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
												 GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8 } GPIO_Mode;

// INIT Functions
void ClockInit(void);
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);												 
void Delay(unsigned int duration);
void USART2_Init(void);

// Functions										 
char USART2_RX(void);
void USART2_TX(char character);
void USART2_TX_String(const char *string);
												 
#endif
