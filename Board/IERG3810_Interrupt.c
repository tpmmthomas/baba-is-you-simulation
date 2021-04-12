#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "IERG3810_Interrupt.h"
#include "IERG3810_LED.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "Board.h"
#include "level.h"


void IERG3810_key2_ExtiInit(void){
	RCC->APB2ENR |= 1<<6;
	GPIOE->CRL &= 0xFFFFF0FF;
	GPIOE->CRL |= 0x00000800;
	GPIOE->ODR |= 1<<2;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFF0FF;
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1<<2;
	EXTI->FTSR |= 1<<2;
	//EXTI->RTSR |= 1<<2;
	NVIC->IP[8] = 0X65;
	NVIC->ISER[0] |= (1<<8);
}

void IERG3810_keyUp_ExtiInit(void){
	RCC->APB2ENR |= 1<<2;
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 0x00000008;
	GPIOA->ODR &= 0xFFFFFFFE;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFFFF0;
	AFIO->EXTICR[0] |= 0x00000000;
	EXTI->IMR |= 1;
	EXTI->FTSR |= 1;
	//EXTI->RTSR |= 1;
	NVIC->IP[6] = 0X35;
	NVIC->ISER[0] |= (1<<6);
}

void IERG3810_KB_ExtiInit(void){
	RCC->APB2ENR |= 1<<4;
	GPIOC->CRH &= 0xFFFF00FF;
	GPIOC->CRH |= 0x00008800;
  GPIOC->ODR |= 1<<11;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[2] &= 0xFFFF0FFF;
	AFIO->EXTICR[2] |= 0x00002000;
	EXTI->IMR |= 1<<11;
	EXTI->FTSR |= 1<<11;
	//EXTI->RTSR |= 1;
	NVIC->IP[40] = 0X65;
	NVIC->ISER[1] |= (1<<8);
}

void EXTI15_10_IRQHandler(void){
	u32 check = GPIOC->IDR;
	if(ps2count == 11){
		Delay(10);
		EXTI->PR = 1<<11;
		return;
	}
	if((check & (1<<10))!= 0)
		ps2key |= 1<<(10-ps2count);
	ps2count++;
	Delay(50);
	EXTI->PR = 1<<11;	
}

void IERG3810_NVIC_SetPriorityGroup(u8 prigroup){
	u32 temp,temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<=8;
	temp = SCB->AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}

void EXTI2_IRQHandler(void){
	GameStatus = 7;
	EXTI->PR = 1<<2;
}

void EXTI0_IRQHandler(void){
	ScreenChange = 1;
	EXTI->PR = 1<<0;
}



void IERG3810_TIM3_Init(u16 arr, u16 psc){
	RCC->APB1ENR |= 1<<1;
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->DIER |= 1<<0;
	TIM3->CR1 |= 0x01;
	NVIC->IP[29] = 0x45;
	NVIC->ISER[0] |= (1<<29);
}

void TIM3_IRQHandler(void){
	if(TIM3->SR & 1<<0){
		if(GameStatus!=0 && GameStatus != 6 && GameStatus != 7 && multi_init_status && !noanim){
			frame = (frame+1)%3;
			if(!updating) animation();
		}
	}
	TIM3->SR &= ~(1<<0);
}

void IERG3810_TIM3_PwmInit(u16 arr, u16 psc){
	RCC->APB2ENR |= 1<<3;
	GPIOB->CRL &= 0xFF0FFFFF;
	GPIOB->CRL |= 0x00B00000;
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR &= 0xFFFFF3FF;
	AFIO->MAPR |= 1<<11;
	RCC->APB1ENR |= 1<<1;
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->CCMR1 |= 7<<12;
	TIM3->CCMR1 |= 1<<11;
	TIM3->CCER |= 1<<4;
	TIM3->CR1 = 0x0080;
	TIM3->CR1 |= 0x01;
}

void IERG3810_TIM4_Init(u16 arr, u16 psc){
	RCC->APB1ENR |= 1<<2;
	TIM4->ARR = arr;
	TIM4->PSC = psc;
	TIM4->DIER |= 1<<0;
	TIM4->CR1 |= 0x01;
	NVIC->IP[30] = 0x40;
	NVIC->ISER[0] |= (1<<30);
}

void TIM4_IRQHandler(void){
	if(TIM4->SR & 1<<0){
		GPIOE->ODR ^= 1<<5;
	}
	TIM4->SR &= ~(1<<0);
}

void IERG3810_SYSTICK_Init100ms(void){
	SysTick->CTRL = 0;
	SysTick->LOAD = 799999;
	SysTick->CTRL |= 3;
}

void USART1_IRQHandler(){
	if(USART1->SR & USART_FLAG_RXNE){
		Received = USART1->DR & 0xFF;
		noanim = 1;
		if(GameStatus == 5) SecondPlayerMove();
		noanim = 0;
		receive_flag = 1;
		USART1->SR &= ~USART_FLAG_RXNE;
	}
}

