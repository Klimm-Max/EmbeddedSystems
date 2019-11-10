#include <stm32l4xx.h>

typedef enum GPIO_Mode { GPIO_INPUT, GPIO_OUTPUT, GPIO_ALTERNATIVE, GPIO_ANALOG,
												 GPIO_INPUT_PULLUP, GPIO_INPUT_PULLDOWN = 0x8 } GPIO_Mode;

void ClockInit(void);
void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode);
												 
int main(void) {
	ClockInit();
	GPIO_Init(GPIOA, 0, GPIO_INPUT_PULLDOWN);														// Initialize the joystick with all 4 directions and pressing it
	GPIO_Init(GPIOA, 1, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 2, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 3, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOA, 5, GPIO_INPUT_PULLDOWN);
	GPIO_Init(GPIOB, 2, GPIO_OUTPUT);																		// Init red lamp on pin 2
	GPIO_Init(GPIOE, 8, GPIO_OUTPUT);
	
	while(1) {
		if (GPIOA->IDR & GPIO_IDR_ID1) GPIOB->BSRR = GPIO_BSRR_BS2;				// If GPIOA data register is set and data register on Pin1 is set -> enable red lamp by enabling pin2
		else if (GPIOA->IDR & GPIO_IDR_ID2) GPIOE->BSRR = GPIO_BSRR_BS8;
		else if (GPIOA->IDR & GPIO_IDR_ID3) GPIOB->BSRR = GPIO_BSRR_BR2;
		else if (GPIOA->IDR & GPIO_IDR_ID5) GPIOE->BSRR = GPIO_BSRR_BR8;
		else if (GPIOA->IDR & GPIO_IDR_ID0) {															// If the joystick is pressed (pin 0) both lamps will be reset
			GPIOB->BSRR = GPIO_BSRR_BR2;
			GPIOE->BSRR = GPIO_BSRR_BR8;
		}
	}
}

void GPIO_Init(GPIO_TypeDef *port, unsigned int pin, GPIO_Mode mode) {
	unsigned int modeIn32Bit	= ((mode & 3) << (2 * pin));							// this will carry any 1 in our desired mode to the correct bit by shifting it 2*pin
	unsigned int pullDown 		= ((mode >> 2) << (2 * pin));
		
	RCC->AHB2ENR |= (1 << (((unsigned int)port - GPIOA_BASE) >> 10));		// RCC is enabling the clock. All GPIOs are connected to AHB2 and controlled by AHB2ENR
																																			// to get the correct port we substract the base adress (adresses start on A up to E)
	port->MODER |= modeIn32Bit;																					// and divide by adress space (1024 or 0x400 -> shifting 10 bits to right is equal)
	port->MODER &= (modeIn32Bit | ~(3 << (2 * pin)));										// set the leftover 0 to the desired pin position
	
	port->PUPDR |= pullDown;																						// set the correct pulldown bits
	port->PUPDR &= (pullDown | ~(3 << (2 * pin)));
}

void ClockInit(void) {
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;																// The flash will have 4 wait states before pushing the next instrc
	
	
	RCC->PLLCFGR 	= RCC_PLLCFGR_PLLREN																	//Enable PLLR that can be used as the system clock
								| (20 << RCC_PLLCFGR_PLLN_Pos)												//Divide the 16MHz input clock by 2(to 8MHz), multiply by 20(to 160MHz),
								| RCC_PLLCFGR_PLLM_0																	//divide by 2(to 80MHz)
								| RCC_PLLCFGR_PLLSRC_HSI;												  		//Set PLL input source to HSI

	RCC->CR |= RCC_CR_PLLON | RCC_CR_HSION;															// enable PLL and use HSI on the Reset-Clock-Controller
	
	while (!((FLASH->ACR & FLASH_ACR_LATENCY_4WS)												//Be sure that the wait state of the flash changed,
					&& (RCC->CR & RCC_CR_PLLRDY)																//PLL circuit is locked, and HSI is stabilized
					&& (RCC->CR & RCC_CR_HSIRDY)));
		
	RCC->CFGR = RCC_CFGR_SW_PLL;																				//Set the system clock source from PLL
	RCC->CR &= ~RCC_CR_MSION;																						//Turn off MSI to reduce power consumption
}