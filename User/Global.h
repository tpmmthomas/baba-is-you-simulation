#ifndef __GLOBAL
#define __GLOBAL
#include "stm32f10x.h"

typedef struct{
	int num_elements;
	char elem[2];
}cell;

extern u8 oneSecCounter;
extern u8 secondCounter;
extern u8 handling;
extern u16 timeTaken;
extern u8 GameStatus;
extern u8 ScreenChange;
extern u8 Received;
extern u8 receive_flag;
extern u8 player;
extern u8 multi_init_status;
extern u32 ps2key;
extern u32 ps2count;
extern cell current_level[12][16];
extern char overlap[12][16];
extern char updated[12][16];
extern u32 current_rules[26];
extern char objects[9];
extern int num_objects;
extern int steps;
extern u8 frame;
extern int flag_frame[3];
extern int baba_frame[3];
extern u8 updating;

#endif
