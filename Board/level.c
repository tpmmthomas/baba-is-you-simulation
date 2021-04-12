#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "level.h"
#include "level_design.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "Board.h"



void level_init(u8 lvl){
	u16 i,j,temp;
	int index;
	updating = 1;
	IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
	if(lvl>4) return;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			index = mapping[levels_board[lvl][i][j]-65];
			if(index >= 0){
				temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
				IERG3810_TFTLCD_ShowImage(j*20,temp,index);
			}
			current_level[i][j].num_elements = 1;
			current_level[i][j].elem[0] = levels_board[lvl][i][j];
			overlap[i][j] = 95;
		}
	}
	steps = 0;
	timeTaken = 0;
	parse_rules();
	updating = 0;
}


void board_update(){
	u16 i,j,k,temp,temp2;
	u8 have_update = 0;
	u8 have_you = 0;
	int index;
	updating = 1;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			for(k=0;k<current_level[i][j].num_elements;k++){
				index = mapping[current_level[i][j].elem[k]-65];
				if((current_level[i][j].elem[k] >=97 && ((current_rules[current_level[i][j].elem[k]-97] & (1<<24)) != 0))||(current_level[i][j].elem[k] >=97 && ((current_rules[current_level[i][j].elem[k]-97] & (1<<20)) != 0))){
					have_you = 1;
			}
				if(index >= 0 && updated[i][j]){
					temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
					temp2 = temp==1?19:20;
					if(k==0) IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
					IERG3810_TFTLCD_ShowImage(j*20,temp,index);
					have_update = 1;
				}
				else if(updated[i][j]){
					temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
					temp2 = temp==1?19:20;
					IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
					have_update = 1;
				}
			}
		}
	}
	if(!have_update && !have_you){
		IERG3810_TFTLCD_FillRectangle(0x6b4d,50,200,220,20);
		IERG3810_TFTLCD_PrintStr(55,222,"Press KEY UP to retry!",0xFFFF);
	}
	updating = 0;
}

void animation(){
		u8 i,j,k;
	u16 temp,temp2;
			for(i=0;i<12;i++){
				for(j=0;j<16;j++){
					for(k=0;k<current_level[i][j].num_elements;k++){
						if(current_level[i][j].elem[k] == 'f'){
							temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
							temp2 = temp==1?19:20;
							if(current_level[i][j].num_elements == 2){
								IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
								if(k==0){
									IERG3810_TFTLCD_ShowImage(j*20,temp,flag_frame[frame]);
									IERG3810_TFTLCD_ShowImage(j*20,temp,mapping[current_level[i][j].elem[1]-65]);
								}
								else{
									IERG3810_TFTLCD_ShowImage(j*20,temp,mapping[current_level[i][j].elem[0]-65]);
									IERG3810_TFTLCD_ShowImage(j*20,temp,flag_frame[frame]);
								}
							}
							else{
								IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
								IERG3810_TFTLCD_ShowImage(j*20,temp,flag_frame[frame]);
							}
						}
						if(current_level[i][j].elem[k] == 'b'){
							temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
							temp2 = temp==1?19:20;
							if(current_level[i][j].num_elements == 2){
								IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
								if(k==0){
									IERG3810_TFTLCD_ShowImage(j*20,temp,baba_frame[frame]);
									IERG3810_TFTLCD_ShowImage(j*20,temp,mapping[current_level[i][j].elem[1]-65]);
								}
								else{
									IERG3810_TFTLCD_ShowImage(j*20,temp,mapping[current_level[i][j].elem[0]-65]);
									IERG3810_TFTLCD_ShowImage(j*20,temp,baba_frame[frame]);
								}
							}
							else{
								IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
								IERG3810_TFTLCD_ShowImage(j*20,temp,baba_frame[frame]);
							}
						}
			}
		}
	}
}

void SecondPlayerMove(){
	u8 pl = 1;
	if(player == 1) pl = 2;
	switch(Received){
			case 0xF1: //down
				down_clicked(pl);
				steps++;
				Delay(100000);
				break;
			case 0xF2: //left
				left_clicked(pl);
				steps++;
				Delay(100000);
				break;
			case 0xF3: //right
				right_clicked(pl);
				steps++;
				Delay(100000);
				break;
			case 0xF4: //up
				up_clicked(pl);
				steps++;
				Delay(100000);
				break;
		}
}
void parse_rules(){ 
	u16 i,j;
	for(i=0;i<26;i++) current_rules[i] = 0;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].elem[current_level[i][j].num_elements-1] >= 65 && current_level[i][j].elem[current_level[i][j].num_elements-1]<= 90 && current_level[i][j].elem[current_level[i][j].num_elements-1] != 73){
					if(j+1<16 && current_level[i][j+1].elem[current_level[i][j+1].num_elements-1] == 73){
						if(j+2<16 && current_level[i][j+2].elem[current_level[i][j+2].num_elements-1] >= 65 && current_level[i][j+2].elem[current_level[i][j+2].num_elements-1]<= 90 && current_level[i][j+2].elem[current_level[i][j+2].num_elements-1] != 73){
							//parse horizontal rule
								current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-65] |= (1<<(current_level[i][j+2].elem[current_level[i][j+2].num_elements-1]-65));
						}
					}
					if(i+1 < 12 && current_level[i+1][j].elem[current_level[i+1][j].num_elements-1] == 73){
						if(i+2<12 && current_level[i+2][j].elem[current_level[i+2][j].num_elements-1] >= 65 && current_level[i+2][j].elem[current_level[i+2][j].num_elements-1]<= 90 && current_level[i+2][j].elem[current_level[i+2][j].num_elements-1] != 73){
							//parse vertical rule
								current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-65] |= (1<<(current_level[i+2][j].elem[current_level[i][j+2].num_elements-1]-65));
						}	
					}
			}
		}
	}	
}

void recursive_push(u16 i,u16 j, u8 dir,u8 enter_flag, u8 pl){
	u8 ch = 24;
	if(pl == 1) ch = 24;
	else ch = 20;
	if(dir == 1){ //left
		if(j==0) return;
		if(current_level[i][j-1].elem[0] == 95){
			current_level[i][j-1].elem[0] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements == 1)
				current_level[i][j].elem[0] = 95;
			else
				current_level[i][j].num_elements--;
			updated[i][j-1] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(enter_flag == 0 &&((current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]>=65 && current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]<=90) || (current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]>=97 && ((current_rules[current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			recursive_push(i,j-1,1,0,pl);
			recursive_push(i,j,1,1,pl);
			return;
		}
		else if(enter_flag == 1 &&((current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]>=65 && current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]<=90) || (current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]>=97 && ((current_rules[current_level[i][j-1].elem[current_level[i][j-1].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			return;
		}
		else if(current_level[i][j-1].elem[0]>=97 && ((current_rules[current_level[i][j-1].elem[0]-97] & (1<<18)) != 0)){ //stop
			return;
		}
		else if(current_level[i][j-1].num_elements == 1 && current_level[i][j-1].elem[0]>=97 && ((current_rules[current_level[i][j-1].elem[0]-97] & (1<<3)) != 0)&& current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){ //defeat
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i][j-1].num_elements == 1 && current_level[i][j-1].elem[0]>=97 && ((current_rules[current_level[i][j-1].elem[0]-97] & (1<<19)) != 0)){ //sink
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			current_level[i][j-1].elem[0] = 95;
			updated[i][j] = 1;
			updated[i][j-1] = 1;
			return;
		}
		else if(current_level[i][j-1].num_elements == 1 && current_level[i][j-1].elem[0]>=97 && ((current_rules[current_level[i][j-1].elem[0]-97] & (1<<22)) != 0) && current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){//win
			current_level[i][j-1].num_elements++;
			current_level[i][j-1].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements --;
			else current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			updated[i][j-1] = 1;
			return;
		}
		else{
			if(current_level[i][j-1].num_elements == 2) return;
			else{
				current_level[i][j-1].num_elements++;
				current_level[i][j-1].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
				if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements--;
				else current_level[i][j].elem[0] = 95;
				updated[i][j]=1;
				updated[i][j-1]=1;
				return;
			}
		}
	}
	else if(dir == 2){ //right
		if(j==15) return;
		if(current_level[i][j+1].elem[0] == 95){
			current_level[i][j+1].elem[0] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements == 1)
				current_level[i][j].elem[0] = 95;
			else
				current_level[i][j].num_elements--;
			updated[i][j+1] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(enter_flag == 0 &&((current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]>=65 && current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]<=90) || (current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]>=97 && ((current_rules[current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			recursive_push(i,j+1,2,0,pl);
			recursive_push(i,j,2,1,pl);
			return;
		}
		else if(enter_flag == 1 &&((current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]>=65 && current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]<=90) || (current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]>=97 && ((current_rules[current_level[i][j+1].elem[current_level[i][j+1].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			return;
		}
		else if(current_level[i][j+1].elem[0]>=97 && ((current_rules[current_level[i][j+1].elem[0]-97] & (1<<18)) != 0)){ //stop
			return;
		}
		else if(current_level[i][j+1].num_elements == 1 && current_level[i][j+1].elem[0]>=97 && ((current_rules[current_level[i][j+1].elem[0]-97] & (1<<3)) != 0)&& current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){ //defeat
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i][j+1].num_elements == 1 && current_level[i][j+1].elem[0]>=97 && ((current_rules[current_level[i][j+1].elem[0]-97] & (1<<19)) != 0)){ //sink
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			current_level[i][j+1].elem[0] = 95;
			updated[i][j] = 1;
			updated[i][j+1] = 1;
			return;
		}
		else if(current_level[i][j+1].num_elements == 1 && current_level[i][j+1].elem[0]>=97 && ((current_rules[current_level[i][j+1].elem[0]-97] & (1<<22)) != 0) && current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){//win
			current_level[i][j+1].num_elements++;
			current_level[i][j+1].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements --;
			else current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			updated[i][j+1] = 1;
			return;
		}
		else{
			if(current_level[i][j+1].num_elements == 2) return;
			else{
				current_level[i][j+1].num_elements++;
				current_level[i][j+1].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
				if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements--;
				else current_level[i][j].elem[0] = 95;
				updated[i][j]=1;
				updated[i][j+1]=1;
			}
		}
	}
	else if(dir == 3){ //up
		if(i==11) return;
		if(current_level[i+1][j].elem[0] == 95){
			current_level[i+1][j].elem[0] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements == 1)
				current_level[i][j].elem[0] = 95;
			else
				current_level[i][j].num_elements--;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(enter_flag == 0 &&((current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]>=65 && current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]<=90) || (current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]>=97 && ((current_rules[current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			recursive_push(i+1,j,3,0,pl);
			recursive_push(i,j,3,1,pl);
			return;
		}
		else if(enter_flag == 1 &&((current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]>=65 && current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]<=90) || (current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]>=97 && ((current_rules[current_level[i+1][j].elem[current_level[i+1][j].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			return;
		}
		else if(current_level[i+1][j].elem[0]>=97 && ((current_rules[current_level[i+1][j].elem[0]-97] & (1<<18)) != 0)){ //stop
			return;
		}
		else if(current_level[i+1][j].num_elements == 1 && current_level[i+1][j].elem[0]>=97 && ((current_rules[current_level[i+1][j].elem[0]-97] & (1<<3)) != 0)&& current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){ //defeat
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i+1][j].num_elements == 1 && current_level[i+1][j].elem[0]>=97 && ((current_rules[current_level[i+1][j].elem[0]-97] & (1<<19)) != 0)){ //sink
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			current_level[i+1][j].elem[0] = 95;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i+1][j].num_elements == 1 && current_level[i+1][j].elem[0]>=97 && ((current_rules[current_level[i+1][j].elem[0]-97] & (1<<22)) != 0) && current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){//win
			current_level[i+1][j].num_elements++;
			current_level[i+1][j].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements --;
			else current_level[i][j].elem[0] = 95;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else{
			if(current_level[i+1][j].num_elements == 2) return;
			else{
				current_level[i+1][j].num_elements++;
				current_level[i+1][j].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
				if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements--;
				else current_level[i][j].elem[0] = 95;
				updated[i+1][j]=1;
				updated[i][j]=1;
			}
		}
	}
	else if(dir == 4){ //down
		if(i==0) return;
		if(current_level[i-1][j].elem[0] == 95){
			current_level[i-1][j].elem[0] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements == 1)
				current_level[i][j].elem[0] = 95;
			else
				current_level[i][j].num_elements--;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(enter_flag == 0 &&((current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]>=65 && current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]<=90) || (current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]>=97 && ((current_rules[current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			recursive_push(i-1,j,4,0,pl);
			recursive_push(i,j,4,1,pl);
			return;
		}
		else if(enter_flag == 1 &&((current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]>=65 && current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]<=90) || (current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]>=97 && ((current_rules[current_level[i-1][j].elem[current_level[i-1][j].num_elements-1]-97] & (1<<15)) != 0)))){//word or push
			return;
		}
		else if(current_level[i-1][j].elem[0]>=97 && ((current_rules[current_level[i-1][j].elem[0]-97] & (1<<18)) != 0)){ //stop
			return;
		}
		else if(current_level[i-1][j].num_elements == 1 && current_level[i-1][j].elem[0]>=97 && ((current_rules[current_level[i-1][j].elem[0]-97] & (1<<3)) != 0)&& current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){ //defeat
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i-1][j].num_elements == 1 && current_level[i-1][j].elem[0]>=97 && ((current_rules[current_level[i-1][j].elem[0]-97] & (1<<19)) != 0)){ //sink
			if(current_level[i][j].num_elements == 2)
					current_level[i][j].num_elements--;
			else	
				current_level[i][j].elem[0] = 95;
			current_level[i-1][j].elem[0] = 95;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i-1][j].num_elements == 1 && current_level[i-1][j].elem[0]>=97 && ((current_rules[current_level[i-1][j].elem[0]-97] & (1<<22)) != 0) && current_level[i][j].elem[current_level[i][j].num_elements-1]>=97 && ((current_rules[current_level[i][j].elem[current_level[i][j].num_elements-1]-97] & ((1<<ch))) != 0)){//win
			current_level[i-1][j].num_elements++;
			current_level[i-1][j].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
			if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements --;
			else current_level[i][j].elem[0] = 95;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
		}	
		else{
			if(current_level[i-1][j].num_elements == 2) return;
			else{
				current_level[i-1][j].num_elements++;
				current_level[i-1][j].elem[1] = current_level[i][j].elem[current_level[i][j].num_elements-1];
				if(current_level[i][j].num_elements ==2) current_level[i][j].num_elements--;
				else current_level[i][j].elem[0] = 95;
				updated[i-1][j]=1;
				updated[i][j]=1;
			}
		}
	}
	
}

void left_clicked(u8 pl){
	u16 i,j,k,l,m;
	u8 ch = 24;
	int to_move[26];
	int total_move = 0;
	if(pl==2) ch = 20;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0 && pl == 1){
			to_move[total_move++] = i;
		}
		if((current_rules[i] & (1<<20)) != 0 && pl == 2){
			to_move[total_move++] = i;
		}
	}
	for(j=0;j<16;j++){
		for(i=0;i<12;i++){
				for(k=0;k<total_move;k++){
					for(l=0;l<current_level[i][j].num_elements;l++){
						if(to_move[k] == current_level[i][j].elem[l]-97){
						//execute moving sequence
							recursive_push(i,j,1,0,pl);
							break;
					}
				}
			}
		}
	}
	
	parse_rules();
	for(i=0;i<num_objects;i++){ //check the rule of becoming another object
		for(j=0;j<num_objects;j++){
			if(i!=j && (current_rules[objects[i]-97] & (1<<(objects[j]-97))) != 0){
				for(k=0;k<12;k++){
					for(l=0;l<16;l++){
						for(m=0;m<current_level[k][l].num_elements;m++){
							if(current_level[k][l].elem[m] == objects[i]){
								current_level[k][l].elem[m] = objects[j];
								updated[k][l] = 1;
							}
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	ch = 20;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	ch = 24;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 6;
			ScreenChange = 1;
		}
	}
}

void right_clicked(u8 pl){
	u16 i,j,k,l,m;
	u8 ch = 24;
	int to_move[26];
	int total_move = 0;
	if(pl==2) ch = 20;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0 && pl == 1){
			to_move[total_move++] = i;
		}
		if((current_rules[i] & (1<<20)) != 0 && pl == 2){
			to_move[total_move++] = i;
		}
	}
	for(j=15;j<=16;j--){
		for(i=0;i<12;i++){
				for(k=0;k<total_move;k++){
					for(l=0;l<current_level[i][j].num_elements;l++){
						if(to_move[k] == current_level[i][j].elem[l]-97){
						//execute moving sequence
							recursive_push(i,j,2,0,pl);
							break;
					}
				}
			}
		}
	}
	
	//check win condition
	parse_rules();
	for(i=0;i<num_objects;i++){
		for(j=0;j<num_objects;j++){
			if(i!=j && (current_rules[objects[i]-97] & (1<<(objects[j]-97))) != 0){
				for(k=0;k<12;k++){
					for(l=0;l<16;l++){
						for(m=0;m<current_level[k][l].num_elements;m++){
							if(current_level[k][l].elem[m] == objects[i]){
								current_level[k][l].elem[m] = objects[j];
								updated[k][l] = 1;
							}
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	ch = 20;
	for(i=0;i<12;i++){//REquirement of >97 fotget!!!!!!!!!!!!!!!!!!!
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	ch = 24;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 6;
			ScreenChange = 1;
		}
	}
}

void down_clicked(u8 pl){
	u16 i,j,k,l,m;
	int to_move[26];
	int total_move = 0;
	u8 ch = 24;
	if(pl==2) ch = 20;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0 && pl == 1){
			to_move[total_move++] = i;
		}
		if((current_rules[i] & (1<<20)) != 0 && pl == 2){
			to_move[total_move++] = i;
		}
	}
	for(i=11;i<=12;i--){
		for(j=0;j<16;j++){
				for(k=0;k<total_move;k++){
					for(l=0;l<current_level[i][j].num_elements;l++){
						if(to_move[k] == current_level[i][j].elem[l]-97){
						//execute moving sequence
							recursive_push(i,j,3,0,pl);
							break;
					}
				}
			}
		}
	}
	
	//check win condition
	parse_rules();
	for(i=0;i<num_objects;i++){
		for(j=0;j<num_objects;j++){
			if(i!=j && (current_rules[objects[i]-97] & (1<<(objects[j]-97))) != 0){
				for(k=0;k<12;k++){
					for(l=0;l<16;l++){
						for(m=0;m<current_level[k][l].num_elements;m++){
							if(current_level[k][l].elem[m] == objects[i]){
								current_level[k][l].elem[m] = objects[j];
								updated[k][l] = 1;
							}
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	ch = 20;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	ch = 24;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 6;
			ScreenChange = 1;
		}
	}
}

void up_clicked(u8 pl){
	u16 i,j,k,l,m;
	int to_move[26];
	int total_move = 0;
	u8 ch = 24;
	if(pl==2) ch = 20;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0 && pl == 1){
			to_move[total_move++] = i;
		}
		if((current_rules[i] & (1<<20)) != 0 && pl == 2){
			to_move[total_move++] = i;
		}
	}
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
				for(k=0;k<total_move;k++){
					for(l=0;l<current_level[i][j].num_elements;l++){
						if(to_move[k] == current_level[i][j].elem[l]-97){
						//execute moving sequence
							recursive_push(i,j,4,0,pl);
							break;
					}
				}
			}
		}
	}
	
	parse_rules();
	for(i=0;i<num_objects;i++){
		for(j=0;j<num_objects;j++){
			if(i!=j && (current_rules[objects[i]-97] & (1<<(objects[j]-97))) != 0){
				for(k=0;k<12;k++){
					for(l=0;l<16;l++){
						for(m=0;m<current_level[k][l].num_elements;m++){
							if(current_level[k][l].elem[m] == objects[i]){
								current_level[k][l].elem[m] = objects[j];
								updated[k][l] = 1;
							}
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	ch = 20;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	ch = 24;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<22)) != 0){
				GameStatus = 6;
				ScreenChange = 1;
			}
			if(current_level[i][j].num_elements == 2 && (current_rules[current_level[i][j].elem[1]-97] & (1<<ch)) != 0 && (current_rules[current_level[i][j].elem[0]-97] & (1<<3)) != 0){
				current_level[i][j].num_elements--;
				board_update();
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 6;
			ScreenChange = 1;
		}
	}
}

