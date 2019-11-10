#include <stm32l4xx.h>

unsigned int sysMillis = 0;

void ClockInit(void);
void TimerInit(void);
void Delay(unsigned int duration);

int main(void) {
	ClockInit();
	TimerInit();
	
	RCC->AHB2ENR = RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;			// enable GPIO Port B and E in general
	GPIOB->MODER &= ~GPIO_MODER_MODE2_1;													// initialize the adress space of Port B thus using Pin 2
	GPIOE->MODER &= ~GPIO_MODER_MODE8_1;													// initialize the adress space of Port E thus using Pin 8

	while(1) {
		GPIOB->BSRR = GPIO_BSRR_BS2;																// BitSetResetRegister of Port B will set Pin 2 to 1 (equal to a Bit sequence but more readable)
		GPIOE->BSRR = GPIO_BSRR_BR8;
		
		Delay(1000);
		
		GPIOB->BSRR = GPIO_BSRR_BR2;																// BitSetResetRegister of Port B will set Pin 2 to 0 (equal to a Bit sequence but more readable)
		GPIOE->BSRR = GPIO_BSRR_BS8;
		
		Delay(1000);
	}
}

void Delay(unsigned int duration) {
	//Temporarily store 'sysMillis' and compare the difference between
	//the value and 'duration' until the value be equal to or bigger than 'duration�
	unsigned int prevMillis = sysMillis;
	while (sysMillis - prevMillis <= duration);
}

void SysTick_Handler(void) { 
	sysMillis++; 
}

void ClockInit(void) {
	FLASH->ACR |= FLASH_ACR_LATENCY_4WS;													// The flash will have 4 wait states before pushing the next instrc
	
	//Enable PLLR that can be used as the system clock
	//Divide the 16MHz input clock by 2(to 8MHz), multiply by 20(to 160MHz),
	//divide by 2(to 80MHz)
	//Set PLL input source to HSI
	RCC->PLLCFGR 	= RCC_PLLCFGR_PLLREN
								| (20 << RCC_PLLCFGR_PLLN_Pos)
								| RCC_PLLCFGR_PLLM_0
								| RCC_PLLCFGR_PLLSRC_HSI;												

	RCC->CR |= RCC_CR_PLLON | RCC_CR_HSION;												// enable PLL and use HSI on the Reset-Clock-Controller
	
	//Be sure that the wait state of the flash changed,
	//PLL circuit is locked, and HSI is stabilized
	while (!((FLASH->ACR & FLASH_ACR_LATENCY_4WS)
					&& (RCC->CR & RCC_CR_PLLRDY)
					&& (RCC->CR & RCC_CR_HSIRDY)));
		
	//Set the system clock source from PLL
	RCC->CFGR = RCC_CFGR_SW_PLL;
	//Turn off MSI to reduce power consumption
	RCC->CR &= ~RCC_CR_MSION;
}

void TimerInit(void) {		
	//Read the calibrated value of 24-bit, and put it in the reload value register
	SysTick->LOAD = SysTick->CALIB & SysTick_LOAD_RELOAD_Msk;
	//Enable SysTick with the exception request
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}