#ifndef __IERG3810_TFTLCD_H
#define __IERG3810_TFTLCD_H
#include "stm32f10x.h"


// put procedure header here
void IERG3810_TFTLCD_Init(void);
void IERG3810_TFTLCD_SetParameter(void);
void IERG3810_TFTLCD_WrReg(u16);
void IERG3810_TFTLCD_WrData(u16);
void IERG3810_TFTLCD_DrawDot(u16,u16,u16);
void IERG3810_TFTLCD_FillRectangle(u16,u16,u16,u16,u16);
void IERG3810_TFTLCD_FillRectangle2(u16,u16,u16,u16,u16);
void IERG3810_TFTLCD_SevenSegment(u16,u16,u16,u8);
void IERG3810_TFTLCD_ShowChar(u16,u16,u8,u16,u16);
void IERG3810_TFTLCD_ShowChar2(u16,u16,u8,u16);
void IERG3810_TFTLCD_ShowImage(u16,u16,u8,u16);
void IERG3810_TFTLCD_PrintStr(u16,u16,char*,u16);
void IERG3810_TFTLCD_ShowChinChar(u16,u16,u8,u16,u16);
void Delay2(u32);
#endif
