#ifndef __LEVEL_H
#define __LEVEL_H
#include "stm32f10x.h"

void level_init(u8);
//u16 rules_var[2]; //0: baba, 1: flag
/*for rules_var:
bit 0: "is you"
bit 1: "is win"
*/
void parse_rules(void);
void left_clicked(void);
void right_clicked(void);
void up_clicked(void);
void down_clicked(void);
void board_update(void);
void recursive_push(u16 i, u16 j,u8 dir);
	
#endif

//currently only flag and baab
