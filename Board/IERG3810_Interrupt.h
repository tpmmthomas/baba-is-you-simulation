#ifndef __IERG3810_INTERRUPT_H
#define __IERG3810_INTERRUPT_H
#include "stm32f10x.h"



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
void IERG3810_SYSTICK_Init100ms(void);
void IERG3810_TIM3_PwmInit(u16,u16);
void USART2_IRQHandler(void);

#endif
