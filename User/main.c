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



#define bgColor 0xEEEE
u8 task1HeartBeat = 0;
u8 i;
char current_level[12][16];
u16 TsX;
u16 TsY;
u8 GameStatus = 0;
u8 ScreenChange = 1;
u32 ps2key = 0;
u32 ps2count = 0;
u8 LastKey = 0;
int main(void){
	//inits
	IERG3810_clock_tree_init();
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
	Delay(1000000);
	while(1){
		if(ps2count>=11){ 
			EXTI->IMR &= ~(1<<11); //No need to receive second break key
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
			case 0x72:
				USART_send(0x11);
				break;
			case 0x6B:
				USART_send(0x21);
				break;
			case 0x74:
				USART_send(0x31);
				break;
			case 0x75:
				USART_send(0x41);
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
			USART_send(0x01);
			if(ScreenChange){
				level_init(0);
				ScreenChange=0;
			}
		}
		if(GameStatus == 2){
			USART_send(0x02);
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"2",0xAFFF);
				ScreenChange=0;
			}
		}
		if(GameStatus == 3){
			USART_send(0x03);
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"3",0xAFFF);
				ScreenChange=0;
			}
		}
		if(GameStatus == 4){
			USART_send(0x04);
			if(ScreenChange){
				IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
				IERG3810_TFTLCD_PrintStr(50,120,"4",0xAFFF);
				ScreenChange=0;
			}
		}
	}
}



