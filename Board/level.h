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
void SecondPlayerMove(void);
void left_clicked(u8);
void right_clicked(u8);
void up_clicked(u8);
void down_clicked(u8);
void board_update(void);
void recursive_push(u16,u16,u8,u8,u8);
void animation(void);
	
#endif

//currently only flag and baab
