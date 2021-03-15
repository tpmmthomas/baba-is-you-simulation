#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include<string.h>
#include "font.h"
#include "cfont.h"
#include "images.h"


typedef struct{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef*) LCD_BASE)

void IERG3810_TFTLCD_Init (void){ //set FSMC
	RCC->AHBENR|=1<<8; //FSMC
	RCC->APB2ENR|=1<<3; //PORTB
	RCC->APB2ENR|=1<<5; //PORTD
	RCC->APB2ENR|=1<<6; //PORTE
	RCC->APB2ENR|=1<<8; //PORTG
	GPIOB->CRL&=0XFFFFFFF0; //PB0
	GPIOB->CRL|=0x00000003;
	
	//PORTD
	GPIOD->CRH&=0X00FFF000;
	GPIOD->CRH|=0XBB000BBB;
	GPIOD->CRL&=0XFF00FF00;
	GPIOD->CRL|=0x00BB00BB;
	
	//PORTE
	GPIOE->CRH&=0x00000000;
	GPIOE->CRH|=0XBBBBBBBB;
	GPIOE->CRL&=0x0FFFFFFF;
	GPIOE->CRL|=0XB0000000;
	
	//PORTG12
	GPIOG->CRH&=0XFFF0FFFF;
	GPIOG->CRH|=0X000B0000;
	GPIOG->CRL&=0xFFFFFFF0; //PGO->RS
	GPIOG->CRL|=0X0000000B;
	
	// LCD uses FSMC Bank 4 memory bank.
	// Use Mode A
	FSMC_Bank1->BTCR[6]=0x00000000; //FSNC_BCR4 (reset)
	FSMC_Bank1->BTCR[7]=0x00000000; //FSMC_BTR4 (reset)
	FSMC_Bank1E->BWTR[6]=0x00000000; //FSMC_BWTR4 (reset)
	FSMC_Bank1->BTCR[6]|=1<<12; //FSMC_BCR4 -> WREN
	FSMC_Bank1->BTCR[6]|=1<<14; //FSMC_BCR4 -> EXTMOD
	FSMC_Bank1->BTCR[6]|=1<<4; //FSMC_BCR4 -> MWID
	FSMC_Bank1->BTCR[7]|=0<<28; //FSMC_BTR4 -> ACCKOD
	FSMC_Bank1->BTCR[7]|=1<<0; //FSMC_BTR4 -> ADDSET
	FSMC_Bank1->BTCR[7]|=0xF<<8;
	FSMC_Bank1E->BWTR[6]|=0<<28; //FSMC_BWIR4 -> ACCMOD
	FSMC_Bank1E->BWTR[6]|=0<<0; //FSMC_BWIR4 -> ADDSET
	FSMC_Bank1E->BWTR[6]|=3<<8; //FSMC_BWIR4 -> DATAST
	FSMC_Bank1->BTCR[6]|=1<<0; //FSNC_BCR4 -> FACCEN
	
	
	IERG3810_TFTLCD_SetParameter(); // special setting for LCD module
	//LCD_LIGHT_ON;
	GPIOB->CRL&=0xFFFFFFF0;
	GPIOB->CRL|=0x00000003;
	GPIOB->BSRR=1;
	
	
}

void IERG3810_TFTLCD_SetParameter(void){
	IERG3810_TFTLCD_WrReg(0x01);
	IERG3810_TFTLCD_WrReg(0x11);
	IERG3810_TFTLCD_WrReg(0x3A);
	IERG3810_TFTLCD_WrData(0x55);
	IERG3810_TFTLCD_WrReg(0x29);
	IERG3810_TFTLCD_WrReg(0x36);
	IERG3810_TFTLCD_WrData(0xAA);
}

void IERG3810_TFTLCD_WrReg(u16 regval){
	LCD->LCD_REG=regval;
}

void IERG3810_TFTLCD_WrData(u16 data){
	LCD->LCD_RAM=data;
}

void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color){
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0x3F);
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData(0x01);
	IERG3810_TFTLCD_WrData(0xDF);
	IERG3810_TFTLCD_WrReg(0x2C);
	IERG3810_TFTLCD_WrData(color);
	Delay2(10);
}

void IERG3810_TFTLCD_FillRectangle(u16 color,u16 start_x, u16 length_x,u16 start_y, u16 length_y){
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(start_x>>8);
	IERG3810_TFTLCD_WrData(start_x & 0xFF);
	IERG3810_TFTLCD_WrData((length_x+start_x-1)>>8);
	IERG3810_TFTLCD_WrData((length_x+start_x-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(start_y>>8);
	IERG3810_TFTLCD_WrData(start_y & 0xFF);
	IERG3810_TFTLCD_WrData((length_y+start_y-1)>>8);
	IERG3810_TFTLCD_WrData((length_y+start_y-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(index = 0;index<length_x*length_y;index++){
			IERG3810_TFTLCD_WrData(color);
	}
}

void IERG3810_TFTLCD_FillRectangle2(u16 color,u16 start_x, u16 length_x,u16 start_y, u16 length_y){
	u16 current_x,current_y;
	for(current_y = start_y;current_y<start_y+length_y;current_y++){
			for(current_x = start_x;current_x<start_x+length_x;current_x++)
				if((current_x & 1) && (current_y & 3))
					IERG3810_TFTLCD_DrawDot(current_x,current_y,color);
	}
}

void IERG3810_TFTLCD_ShowChar(u16 x,u16 y,u8 ascii,u16 color,u16 bgcolor){
	u8 i,j;
	u8 index;
	u8 height=16,length=8;
	if(ascii<32 || ascii>127) return;
	ascii-=32;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData((length+x-1)>>8);
	IERG3810_TFTLCD_WrData((length+x-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData((height+y-1)>>8);
	IERG3810_TFTLCD_WrData((height+y-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(j=0;j<height/8;j++){
		for(i=0;i<height/2;i++){
			for(index=0;index<length;index++){
				if((asc2_1608[ascii][index*2+1-j]>>i) & 1) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
}

void IERG3810_TFTLCD_ShowChar2(u16 x,u16 y,u8 ascii,u16 color){ //3.6 bonus
	u8 i,j;
	u8 index;
	u8 height=16,length=8;
	u8 current_x=x,current_y=y;
	if(ascii<32 || ascii>127) return;
	ascii-=32;
	for(j=0;j<height/8;j++){
		for(i=0;i<height/2;i++){
			for(index=0;index<length;index++){
				if((asc2_1608[ascii][index*2+1-j]>>i) & 1) IERG3810_TFTLCD_DrawDot(current_x,current_y,color);
				current_x++;
				if(current_x>=x+length){
					current_x=x;
					current_y++;
				}
			}
		}
	}
}

void IERG3810_TFTLCD_ShowImage(u16 x,u16 y,u8 idx){ 
	u8 i,j;
	u8 height=20,length=20;
	u8 current_x=x,current_y=y+height-1;
	if(idx>2) return; //keep changing
	for(i = 0; i<height*length/8;i++){
		for(j=8;j>0;j--){
			if((images[idx][i]>>(j-1)) & 1) IERG3810_TFTLCD_DrawDot(current_x,current_y,images_color[idx]);
			current_y--;
			if(current_y<y){
					current_y = y+height-1;
					current_x++;
			}
		}
	}
}



void IERG3810_TFTLCD_ShowChinChar(u16 x,u16 y,u8 ind,u16 color,u16 bgcolor){
	u8 i,j;
	u8 index;
	u8 height=16,length=16;
	if(ind>3) return;
	IERG3810_TFTLCD_WrReg(0x2A);
	IERG3810_TFTLCD_WrData(x>>8);
	IERG3810_TFTLCD_WrData(x & 0xFF);
	IERG3810_TFTLCD_WrData((length+x-1)>>8);
	IERG3810_TFTLCD_WrData((length+x-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
	IERG3810_TFTLCD_WrData(y>>8);
	IERG3810_TFTLCD_WrData(y & 0xFF);
	IERG3810_TFTLCD_WrData((height+y-1)>>8);
	IERG3810_TFTLCD_WrData((height+y-1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(j=0;j<height/8;j++){
		for(i=0;i<height/2;i++){
			for(index=0;index<length;index++){
				if((chi_1616[ind][index*2+1-j]>>i) & 1) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
}

void IERG3810_TFTLCD_PrintStr(u16 x,u16 y,char* str,u16 color){
	u8 i;
	for(i=0;i<strlen(str);i++){
				IERG3810_TFTLCD_ShowChar2(x+i*8,y,str[i],color);
				Delay2(10000);
		}
}

void Delay2(u32 count){
	u32 i;
	for(i=0; i<count; i++);
}

	

void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y,u8 digit){
	IERG3810_TFTLCD_SetParameter();
	switch(digit){
		case 0:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+10,65);//e
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			break;
		case 1:
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			break;
		case 2:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+10,65);//e
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
		 break;
		case 3:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
		case 4:
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
		case 5:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
		case 6:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+10,65);//e
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
		case 7:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			break;
		case 8:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+10,65);//e
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
		case 9:
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+150,10); //a
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+85,65);//b
			IERG3810_TFTLCD_FillRectangle(color,start_x+70,10,start_y+10,65);//c
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y,10); //d
			IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y+85,65);//f
			IERG3810_TFTLCD_FillRectangle(color,start_x+10,60,start_y+75,10);//g
			break;
	}
}


