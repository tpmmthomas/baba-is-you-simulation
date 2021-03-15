#include "stm32f10x.h"
#include "IERG3810_Buzzer.h"

// put your procedure and code here
void IERG3810_Buzzer_Init(){
	RCC->APB2ENR |= 1<<3;
	GPIOB->CRH &= 0xFFFFFFF0;
	GPIOB->CRH |= 0x00000003;
}

u8 BuzzerIsSounded(){
	u32 BuzzerOut = GPIOB->ODR;
	u32 bz = 1<<8;
	if((BuzzerOut & bz) != 0)
		return 1;
	return 0;
}

void SetBuzzerOn(){
	GPIOB->BSRR = 1<<8;
}

void SetBuzzerOff(){
	GPIOB->BRR = 1<<8;
}
