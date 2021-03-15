#ifndef __IERG3810_KEY_H
#define __IERG3810_KEY_H
#include "stm32f10x.h"

// put procedure header here
void IERG3810_KEY_Init(void);
u8 Key0IsPressed(void);
u8 Key1IsPressed(void);
u8 Key2IsPressed(void);
u8 KeyUpIsPressed(void);


#endif
