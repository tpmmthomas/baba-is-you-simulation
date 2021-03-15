#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_TFTLCD.h"
#include "stm32f10x_it.h"
#include<string.h>

void Delay(u32);
void IERG3810_key2_ExtiInit(void);
void IERG3810_keyUp_ExtiInit(void);
void IERG3810_NVIC_SetPriorityGroup(u8);
void EXTI2_IRQHandler(void);
void EXTI0_IRQHandler(void);
void IERG3810_KB_ExtiInit(void);
void EXTI15_10_IRQHandler(void);
void IERG3810_TIM3_Init(u16,u16);
void TIM3_IRQHandler(void);
void IERG3810_TIM4_Init(u16,u16);
void TIM4_IRQHandler(void);
void IERG3810_SYSTICK_Init10ms(void);
void IERG3810_TIM3_PwmInit(u16,u16);

#define bgColor 0xEEEE
u8 task1HeartBeat = 0;
u8 cycle = 0;
u8 led0Done[10];
u8 led1Done[10];
u8 i;
int main(void){
	u16 led0pwmval = 0;
	u8 dir = 1;
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	SetLight1Off();
	SetLight0Off();
	IERG3810_TIM3_PwmInit(6666,72);
	IERG3810_TIM4_Init(1249,7199);
	while(1){
		Delay(1500);
		if(dir) led0pwmval++;
		else led0pwmval--;
		if(led0pwmval > 7000) dir = 0;
		if(led0pwmval == 0) dir = 1;
		TIM3->CCR2 = led0pwmval;
		
	}
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
	GPIOB->BRR = 1<<5;
	GPIOB->BSRR = 1<<5;
	GPIOB->BRR = 1<<5;
	GPIOB->BSRR = 1<<5;
	GPIOB->ODR ^= 1<<5;
	GPIOB->ODR ^= 1<<5;
	GPIOB->ODR ^= 1<<5;
	GPIOB->ODR ^= 1<<5;
	GPIOB->ODR &= ~(1<<5);
	GPIOB->ODR |= 1<<5;
	GPIOB->ODR &= ~(1<<5);
	GPIOB->ODR |= 1<<5;
	TIM3->SR &= ~(1<<0);
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

void IERG3810_SYSTICK_Init10ms(void){
	SysTick->CTRL = 0;
	SysTick->LOAD = 79999;
	SysTick->CTRL |= 3;
}

void Delay(u32 count){
	u32 i;
	for(i=0; i<count; i++);
}


