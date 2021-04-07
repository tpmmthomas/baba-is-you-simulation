#ifndef __GLOBAL
#define __GLOBAL
#include "stm32f10x.h"

extern u8 task1HeartBeat;
extern u8 GameStatus;
extern u8 ScreenChange;
extern u32 ps2key;
extern u32 ps2count;
extern char current_level[12][16];
extern char overlap[12][16];
extern u32 current_rules[26];
#endif
