#include "stm32f10x.h"
#include "IERG3810_LED.h"

void IERG3810_LED_Init(){
	RCC->APB2ENR |= 1<<3;
	RCC->APB2ENR |= 1<<6;
	GPIOB->CRL &= 0xFF0FFFFF;//LED_0
	GPIOB->CRL |= 0x00300000; //push-pull mode
	GPIOE->CRL &= 0xFF0FFFFF;//LED_1
	GPIOE->CRL |= 0x00300000;
}

u8 Light0IsOn(){
	u32 light0out = GPIOB->ODR;
	u32 l0 = 1<<5;
	if((light0out & l0) == 0)
		return 1;
	return 0;
}
u8 Light1IsOn(void){
	u32 light1out = GPIOE->ODR;
	u32 l1 = 1<<5;
	if((light1out & l1) == 0)
		return 1;
	return 0;
}

void SetLight0On(){
	GPIOB->BRR = 1<<5;
}
void SetLight1On(){
	GPIOE->BRR = 1<<5;
}
void SetLight0Off(){
	GPIOB->BSRR = 1<<5;
}
void SetLight1Off(){
	GPIOE->BSRR = 1<<5;
}

