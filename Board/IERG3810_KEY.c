#include "stm32f10x.h"
#include "IERG3810_KEY.h"

void IERG3810_KEY_Init(){
	RCC->APB2ENR |= 1<<2;
	RCC->APB2ENR |= 1<<6;
	GPIOA->CRL &= 0xFFFFFFF0;//key_UP
	GPIOA->CRL |= 0x00000008;//key_UP (input mode)
	GPIOE->CRL &= 0xFFF000FF;//key 0,1,2
	GPIOE->CRL |= 0x00088800;
	GPIOA->ODR &= 0xFFFFFFFE; //pull-down mode
	GPIOE->ODR |= 1<<2; //Pull-up mode 
	GPIOE->ODR |= 1<<3; 
	GPIOE->ODR |= 1<<4;
}	

u8 Key0IsPressed(){
	u32 keysIn012 = GPIOE->IDR;
	u32 x0 = 1<<4;
	if((keysIn012 & x0) == 0)
		return 1;
	return 0;
}

u8 Key1IsPressed(){
	u32 keysIn012 = GPIOE->IDR;
	u32 x1 = 1<<3;
	if((keysIn012 & x1) == 0)
		return 1;
	return 0;
}

u8 Key2IsPressed(){
	u32 keysIn012 = GPIOE->IDR;
	u32 x2 = 1<<2;
	if((keysIn012 & x2) == 0)
		return 1;
	return 0;
}

u8 KeyUpIsPressed(){
	u32 keysInUp = GPIOA->IDR;
	u32 xU = 1;
	if((keysInUp & xU) != 0)
		return 1;
	return 0;
}
