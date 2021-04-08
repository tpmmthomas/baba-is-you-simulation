#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_TouchScreen.h"
#include "stm32f10x_it.h"
#include "IERG3810_Interrupt.h"
#include "Board.h"
#include "level.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>



#define bgColor 0xEEEE
u8 oneSecCounter = 0;
u16 timeTaken = 0;
u8 i;
u8 secondCounter = 0;
cell current_level[12][16];
char overlap[12][16];
char updated[12][16];
char objects[7] = {'b','f','g','v','r','j','q'};
int num_objects = 7;
char marks[10] = "Steps:";
char times[10] = "Time:";
u32 current_rules[26];
u16 TsX;
u16 TsY;
u8 GameStatus = 0;
u8 ScreenChange = 1;
u32 ps2key = 0;
u32 ps2count = 0;
u8 LastKey = 0;
int steps = 0;
int main(void){
	//inits
	IERG3810_clock_tree_init();
	IERG3810_SYSTICK_Init100ms();
	IERG3810_LED_Init();
	IERG3810_USART1_init(72,9600);
	IERG3810_USART2_init(36,9600);
	IERG3810_TouchScreen_init();
	IERG3810_TFTLCD_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	IERG3810_keyUp_ExtiInit();
	IERG3810_KB_ExtiInit();
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
	IERG3810_TFTLCD_PrintStr(65,80,"Press anywhere to begin!",0x07FF);
	for(i=0;i<12;i++)
		IERG3810_TFTLCD_ShowImage(40+20*i,20,i);
	Delay(100000);
	do{
		TsX = TouchScreenReadData(5);
		TsY = TouchScreenReadData(1);
		Delay(1000);
	}while((TsX/10000) == 0 && (TsY/10000) == 0);
	Delay(2500000);
	while(1){
		if(ps2count>=11){ 
			EXTI->IMR &= ~(1<<11); //No need to receive second break key
			USART_send(0xFF);
			for(i=8;i>=1;i--){
				if((ps2key & (1<<(10-i))) != 0){
					LastKey |= 1<<(i-1);
				}
			}		
			ps2count = 0;
			ps2key = 0;
			EXTI->IMR |= (1<<11);
		}
		switch(LastKey){
			case 0x73: //up
				EXTI->IMR &= ~(1<<11);
				USART_send(0x11);
				up_clicked();
				steps++;
				Delay(1000000);
				EXTI->IMR |= (1<<11);
				ps2count = 0;
				ps2key = 0;
				break;
			case 0x6B: //left
				EXTI->IMR &= ~(1<<11);
				USART_send(0x21);
				left_clicked();
				steps++;
				Delay(1000000);
				EXTI->IMR |= (1<<11);
				ps2count = 0;
				ps2key = 0;
				break;
			case 0x74: //right
				EXTI->IMR &= ~(1<<11);
				USART_send(0x31);
				right_clicked();
				steps++;
				Delay(1000000);
				EXTI->IMR |= (1<<11);
				ps2count = 0;
				ps2key = 0;
				break;
			case 0x75: //down
				EXTI->IMR &= ~(1<<11);
				USART_send(0x41);
				down_clicked();
				steps++;
				Delay(1000000);
				EXTI->IMR |= (1<<11);
				ps2count = 0;
				ps2key = 0;
				break;
			default:
				if(ps2count>=11){
					ps2count = 0;
					ps2key = 0;
				}
				break;
		}
		LastKey = 0;
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
			if(TsX>=10700 && TsX<=10900){
				Delay(10000); //decrease sensitivity
				TsX = TouchScreenReadData(5);
				if(TsX>=10700 && TsX<=10900){
					TsY = TouchScreenReadData(1);
					if(TsY>=11560 && TsY<=11760)
							GameStatus = 1;
					else if(TsY>=11160 && TsY<=11360)
						GameStatus = 2;
					else if(TsY>=10760 && TsY<=10960)
						GameStatus = 3;
					else if(TsY>=10360 && TsY<=10560)
						GameStatus = 4;
				}
			}
			if(GameStatus != 0) ScreenChange = 1;
		}
		if(GameStatus == 1){
			if(ScreenChange){
				level_init(0);
				ScreenChange=0;
			}
		}
		if(GameStatus == 2){
			if(ScreenChange){
				level_init(1);
				ScreenChange=0;
			}
		}
		if(GameStatus == 3){
			if(ScreenChange){
				level_init(2);
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
		if(GameStatus==5){
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x328a,100,100,80,80);
				IERG3810_TFTLCD_PrintStr(115,140,"You win!",0xFFFF);
				sprintf(marks+6,"%d",steps);
				IERG3810_TFTLCD_PrintStr(110,120,marks,0xFFFF);
				sprintf(times+5,"%d",timeTaken);
				strcat(times,"s");
				IERG3810_TFTLCD_PrintStr(110,100,times,0xFFFF);
				ScreenChange=0;
				secondCounter = 0;
			}
			if(secondCounter>=3){
				GameStatus = 0;
				ScreenChange = 1;
			}
		}
	}
}



