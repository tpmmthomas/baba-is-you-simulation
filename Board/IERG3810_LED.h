#ifndef __IERG3810_LED_H
#define __IERG3810_LED_H
#include "stm32f10x.h"

// put procedure header here
void IERG3810_LED_Init(void);
u8 Light0IsOn(void);
u8 Light1IsOn(void);
void SetLight0On(void); //explicitly define for every LED, since there are only two!
void SetLight1On(void);
void SetLight0Off(void);
void SetLight1Off(void);

#endif
