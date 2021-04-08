#ifndef __GLOBAL
#define __GLOBAL
#include "stm32f10x.h"

typedef struct{
	int num_elements;
	char elem[2];
}cell;

extern u8 oneSecCounter;
extern u8 secondCounter;
extern u16 timeTaken;
extern u8 GameStatus;
extern u8 ScreenChange;
extern u32 ps2key;
extern u32 ps2count;
extern cell current_level[12][16];
extern char overlap[12][16];
extern char updated[12][16];
extern u32 current_rules[26];
extern char objects[7];
extern int num_objects;
extern int steps;


#endif
