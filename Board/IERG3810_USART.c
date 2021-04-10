#include "stm32f10x.h"
#include "IERG3810_USART.h"


void IERG3810_USART2_init(u32 pclk1, u32 bound){
	//float temp;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//u16 mantissa;
	//u16 fraction;
	//temp = (float)(pclk1*1000000)/(bound*16);
	//mantissa=temp;
	//fraction=(temp-mantissa)*16;
	//mantissa<<=4;
	//mantissa+=fraction;
	RCC->APB2ENR|=1<<2;
	RCC->APB1ENR|=1<<17;
	GPIOA->CRL&=0xFFFF00FF;
	GPIOA->CRL|=0x00008B00;
	RCC->APB1RSTR|=1<<17;
	RCC->APB1RSTR&=~(1<<17);
	//USART2->BRR=mantissa;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx| USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE , ENABLE);
	USART2->CR1 = 0x202C;
	USART2->CR2 &= (3<<12);
}

void IERG3810_USART1_init(u32 pclk1, u32 bound){
	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1*1000000)/(bound*16);
	mantissa=temp;
	fraction=(temp-mantissa)*16;
	mantissa<<=4;
	mantissa+=fraction;
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<14;
	GPIOA->CRH&=0xFFFFF00F;
	GPIOA->CRH|=0x000008B0;
	RCC->APB2RSTR|=1<<14;
	RCC->APB2RSTR&=~(1<<14);
	USART1->BRR=mantissa;
	NVIC->ISER[1] |= (1<<5);
	USART1->CR1|=0x202C;
}

void USART_print(u8 USARTport, char *st){
	u8 i=0;
	u32 temp = 1<<7;
	while(st[i]!=0){
		if(USARTport == 1){
			USART1->DR = st[i];
			while(!(temp & USART1->SR));
		}
		if(USARTport == 2){
			USART2->DR = st[i];
			while(!(temp & USART2->SR));
		}
		if(i==255) break;
		i++;
	}
}

void USART_send(u8 st){
		u32 temp = 1<<7;
		USART1->DR = st;
		while(!(temp & USART1->SR));
}
