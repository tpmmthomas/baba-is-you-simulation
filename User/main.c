#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_TouchScreen.h"
#include "stm32f10x_it.h"
#include "Board.h"
#include<string.h>

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
u8 i;

u16 TsX;
u16 TsY;
u8 GameStatus = 0;
u8 ScreenChange = 1;
char position[8]="-----\0";
int main(void){
	//inits
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_USART1_init(72,9600);
	IERG3810_USART2_init(36,9600);
	IERG3810_TouchScreen_init();
	IERG3810_TFTLCD_Init();
	Delay(1000000);
	SetLight0Off();
	SetLight1Off();
	//starting screen
	IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
	Delay(10000);
	IERG3810_TFTLCD_PrintStr(70,200,"Welcome to ",0xFFFF);
	IERG3810_TFTLCD_PrintStr(158,200,"Baba is You",0xF800);
	IERG3810_TFTLCD_PrintStr(65,160,"Lui Kwan Kin 1155110469",0xFFFF);
	IERG3810_TFTLCD_PrintStr(65,140,"Wong Wan Ki 1155124843",0xFFFF);
	IERG3810_TFTLCD_PrintStr(65,80,"Press anywhere to begin",0x07FF);
	IERG3810_TFTLCD_FillRectangle(0x3400,20,20,20,20);
	IERG3810_TFTLCD_ShowImage(20,20,0);
	Delay(100000);
	do{
		TsX = TouchScreenReadData(5);
		TsY = TouchScreenReadData(1);
		Delay(1000);
	}while((TsX/10000) == 0 && (TsY/10000) == 0);
	Delay(100000);
	while(1){
		if(GameStatus == 0){
			/* level selection screen */
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(65,180,"Please choose the level",0xFFFF);
				IERG3810_TFTLCD_PrintStr(50,120,"1",0xAFFF);
				IERG3810_TFTLCD_PrintStr(110,120,"2",0xFBFF);
				IERG3810_TFTLCD_PrintStr(170,120,"3",0xFFCF);
				IERG3810_TFTLCD_PrintStr(230,120,"4",0xFFFD);
				ScreenChange=0;
			}
			TsX = TouchScreenReadData(5);
			TsY = TouchScreenReadData(1);
			if(TsX>=10700 && TsX<=10900){
				if(TsY>=11560 && TsY<=11760)
						GameStatus = 1;
				else if(TsY>=11160 && TsY<=11360)
					GameStatus = 2;
				else if(TsY>=10760 && TsY<=10960)
					GameStatus = 3;
				else if(TsY>=10360 && TsY<=10560)
					GameStatus = 4;
			}
			if(GameStatus != 0) ScreenChange = 1;
		}
		if(GameStatus == 1){
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"1",0xAFFF);
				ScreenChange=0;
			}
		}
		if(GameStatus == 2){
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"2",0xAFFF);
				ScreenChange=0;
			}
		}
		if(GameStatus == 3){
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"3",0xAFFF);
				ScreenChange=0;
			}
		}
		if(GameStatus == 4){
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"4",0xAFFF);
				ScreenChange=0;
			}
		}
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


