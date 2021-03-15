#ifndef __IERG3810_BUZZER_H
#define __IERG3810_BUZZER_H
#include "stm32f10x.h"

// put procedure header here
void IERG3810_Buzzer_Init(void);
u8 BuzzerIsSounded(void);
void SetBuzzerOn(void);
void SetBuzzerOff(void);

#endif
