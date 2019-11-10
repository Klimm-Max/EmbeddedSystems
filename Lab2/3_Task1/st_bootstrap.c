#include "st_bootstrap.h"

unsigned int sysMillis = 0;

// ============================== FUNCTIONS ================================
void Delay(unsigned int duration) {
	//Temporarily store 'sysMillis' and compare the difference between
	//the value and 'duration' until the value be equal to or bigger than 'duration’
	unsigned int prevMillis = sysMillis;
	while (sysMillis - prevMillis <= duration);
}

char USART2_RX(void) {
	while (!(USART2->ISR & USART_ISR_RXNE));														// just wait until USART is ready for reading (RXNE)
	return USART2->RDR;																									// return the RecievedDataRegister of USART2
}

void USART2_TX(char character) {
	while (!(USART2->ISR & USART_ISR_TXE));															// just wait until USART is ready for a transcieving operation
	USART2->TDR = character;																						// write character to TranscievingDataRegister
}

void USART2_TX_String(const char *string) {
	while (*string != '\0') USART2_TX(*string++);												// As long as the string is not 0 terminated, transcieve the currecnt character and increment
}

// =========================== INIT FUNCTIONS ============================== 
void USART2_Init(void) {
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_1;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;															// enabling USART2 over the APB1ENR1 controller from RCC register
	
	GPIO_Init(GPIOD, 5, GPIO_ALTERNATIVE);
	GPIO_Init(GPIOD, 6, GPIO_ALTERNATIVE);
	GPIOD->AFR[0]	|= (7 << GPIO_AFRL_AFSEL6_Pos) 												// set the Alternate function for pin5/6 to function number 7
								|  (7 << GPIO_AFRL_AFSEL5_Pos);
	
	USART2->BRR = 139;																									// 16MHz HSI clock with 115200 Baud rate (16*10^6/115200 ~ 139)
	USART2->CR3 |= USART_CR3_OVRDIS;																		// dealing with overflow
	USART2->CR1 = USART_CR1_TE | USART_CR1_RE |USART_CR1_UE;						// enabling USART for recieving
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

void SysTickInit(void) {
	SysTick->LOAD = SysTick->CALIB & SysTick_LOAD_RELOAD_Msk;						//Read the calibrated value of 24-bit, and put it in the reload value register
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;	//Enable SysTick with the exception request
}
