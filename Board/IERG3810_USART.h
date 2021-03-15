#ifndef __IERG3810_USART_H
#define __IERG3810_USART_H
#include "stm32f10x.h"

// put procedure header here
void USART_print(u8 USARTport, char *st);
void IERG3810_USART1_init(u32 pclk1, u32 bound);
void IERG3810_USART2_init(u32 pclk1, u32 bound);

#endif
