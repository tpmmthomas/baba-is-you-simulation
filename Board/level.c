#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "level.h"
#include "level_design.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"

void level_init(u8 lvl){
	u16 i,j,temp;
	int index;
	IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
	if(lvl>4) return;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			index = mapping[levels_board[lvl][i][j]-65];
			if(index >= 0){
				temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
				IERG3810_TFTLCD_ShowImage(j*20,temp,index);
			}
			current_level[i][j] = levels_board[lvl][i][j];
			overlap[i][j] = 95;
		}
	}
	steps = 0;
	parse_rules();
}


void board_update(){
	u16 i,j,temp,temp2;
	int index;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			index = mapping[current_level[i][j]-65];
			if(index >= 0 && updated[i][j]){
				temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
				temp2 = temp==1?19:20;
				IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
				IERG3810_TFTLCD_ShowImage(j*20,temp,index);
			}
			else if(updated[i][j]){
				temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
				temp2 = temp==1?19:20;
				IERG3810_TFTLCD_FillRectangle(0x0,j*20,20,temp,temp2);
			}
			index = mapping[overlap[i][j]-65];
			if(index >= 0 && updated[i][j]){
				temp = i==11?1:(11-i)*20; //writing to y=0 cause problems
				IERG3810_TFTLCD_ShowImage(j*20,temp,index);
			}
		}
	}
}

void parse_rules(){ 
	u16 i,j;
	for(i=0;i<26;i++) current_rules[i] = 0;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(current_level[i][j] >= 65 && current_level[i][j]<= 90 && current_level[i][j] != 73){
					if(j+1<16 && current_level[i][j+1] == 73){
						if(j+2<16 && current_level[i][j+2] >= 65 && current_level[i][j+2]<= 90 && current_level[i][j+2] != 73){
							//parse horizontal rule
								current_rules[current_level[i][j]-65] |= (1<<(current_level[i][j+2]-65));
						}
					}
					if(i+1 < 12 && current_level[i+1][j] == 73){
						if(i+2<12 && current_level[i+2][j] >= 65 && current_level[i+2][j]<= 90 && current_level[i+2][j] != 73){
							//parse vertical rule
								current_rules[current_level[i][j]-65] |= (1<<(current_level[i+2][j]-65));
						}	
					}
			}
		}
	}	
}

void recursive_push(u16 i,u16 j, u8 dir){
	if(dir == 1){ //left
		if(j==0) return;
		if(current_level[i][j-1] == 95){
			current_level[i][j-1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j-1] = 1;
			updated[i][j] = 1;
			return;
		}
		else if((current_level[i][j-1]>=65 && current_level[i][j-1]<=90) || (current_level[i][j-1]>=97 && ((current_rules[current_level[i][j-1]-97] & (1<<15)) != 0))){//word or push
			recursive_push(i,j-1,1);
			if(current_level[i][j-1] == 95){
			current_level[i][j-1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j-1] = 1;
			updated[i][j] = 1;
			return;
			}
		}
		else if(current_level[i][j-1]>=97 && ((current_rules[current_level[i][j-1]-97] & (1<<19)) != 0)){ //sink
			current_level[i][j] = 95;
			current_level[i][j-1] = 95;
			updated[i][j] = 1;
			updated[i][j-1] = 1;
			return;
		}
		else if(current_level[i][j-1]>=97 && ((current_rules[current_level[i][j-1]-97] & (1<<22)) != 0) && current_level[i][j]>=97 && ((current_rules[current_level[i][j]-97] & (1<<24)) != 0)){//win
			overlap[i][j-1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j] = 1;
			updated[i][j-1] = 1;
			return;
		}
		else return;
	}
	else if(dir == 2){ //right
		if(j==15) return;
		if(current_level[i][j+1] == 95){
			current_level[i][j+1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j+1] = 1;
			updated[i][j] = 1;
			return;
		}
		else if((current_level[i][j+1]>=65 && current_level[i][j+1]<=90) || (current_level[i][j+1]>=97 && ((current_rules[current_level[i][j+1]-97] & (1<<15)) != 0))){
			recursive_push(i,j+1,2);
			if(current_level[i][j+1] == 95){
			current_level[i][j+1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j+1] = 1;
			updated[i][j] = 1;
			return;
			}
		}
		else if(current_level[i][j+1]>=97 && ((current_rules[current_level[i][j+1]-97] & (1<<19)) != 0)){ //sink
			current_level[i][j] = 95;
			current_level[i][j+1] = 95;
			updated[i][j+1] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i][j+1]>=97 && ((current_rules[current_level[i][j+1]-97] & (1<<22)) != 0) && current_level[i][j]>=97 && ((current_rules[current_level[i][j]-97] & (1<<24)) != 0)){//win
			overlap[i][j+1] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j] = 1;
			updated[i][j+1] = 1;
			return;
		}
		else return;
	}
	else if(dir == 3){ //up
		if(i==11) return;
		if(current_level[i+1][j] == 95){
			current_level[i+1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if((current_level[i+1][j]>=65 && current_level[i+1][j]<=90) || (current_level[i+1][j]>=97 && ((current_rules[current_level[i+1][j]-97] & (1<<15)) != 0))){
			recursive_push(i+1,j,3);
			if(current_level[i+1][j] == 95){
			current_level[i+1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
			}
		}
		else if(current_level[i+1][j]>=97 && ((current_rules[current_level[i+1][j]-97] & (1<<19)) != 0)){ //sink
			current_level[i][j] = 95;
			current_level[i+1][j] = 95;
			updated[i+1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i+1][j]>=97 && ((current_rules[current_level[i+1][j]-97] & (1<<22)) != 0) && current_level[i][j]>=97 && ((current_rules[current_level[i][j]-97] & (1<<24)) != 0)){//win
			overlap[i+1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j] = 1;
			updated[i+1][j] = 1;
			return;
		}
		else return;
	}
	else if(dir == 4){ //up
		if(i==0) return;
		if(current_level[i-1][j] == 95){
			current_level[i-1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if((current_level[i-1][j]>=65 && current_level[i-1][j]<=90) || (current_level[i-1][j]>=97 && ((current_rules[current_level[i-1][j]-97] & (1<<15)) != 0))){
			recursive_push(i-1,j,4);
			if(current_level[i-1][j] == 95){
			current_level[i-1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
			}
		}
		else if(current_level[i-1][j]>=97 && ((current_rules[current_level[i-1][j]-97] & (1<<19)) != 0)){ //sink
			current_level[i][j] = 95;
			current_level[i-1][j] = 95;
			updated[i-1][j] = 1;
			updated[i][j] = 1;
			return;
		}
		else if(current_level[i-1][j]>=97 && ((current_rules[current_level[i-1][j]-97] & (1<<22)) != 0) && current_level[i][j]>=97 && ((current_rules[current_level[i][j]-97] & (1<<24)) != 0)){//win
			overlap[i-1][j] = current_level[i][j];
			current_level[i][j] = 95;
			updated[i][j] = 1;
			updated[i-1][j] = 1;
			return;
		}
		else return;
	}
	
}
//For simplicity, oodo not allow overlap unless win!!!
void left_clicked(){
	u16 i,j,k,l;
	int to_move[26];
	int total_move = 0;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0){
			to_move[total_move++] = i;
		}
	}
	for(j=0;j<16;j++){
		for(i=0;i<12;i++){
				for(k=0;k<total_move;k++){
					if(to_move[k] == current_level[i][j]-97){
						//execute moving sequence
						recursive_push(i,j,1);
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
						if(current_level[k][l] == objects[i]){
							current_level[k][l] = objects[j];
							updated[k][l] = 1;
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(overlap[i][j] != 95){
				GameStatus = 5;
				ScreenChange = 1;
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 5;
			ScreenChange = 1;
		}
	}
}

void right_clicked(){
	u16 i,j,k,l;
	int to_move[26];
	int total_move = 0;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0){
			to_move[total_move++] = i;
		}
	}
	for(j=15;j<=16;j--){
		for(i=0;i<12;i++){
				for(k=0;k<total_move;k++){
					if(to_move[k] == current_level[i][j]-97){
						//execute moving sequence
						recursive_push(i,j,2);
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
						if(current_level[k][l] == objects[i]){
							current_level[k][l] = objects[j];
							updated[k][l] = 1;
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(overlap[i][j] != 95){
				GameStatus = 5;
				ScreenChange = 1;
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 5;
			ScreenChange = 1;
		}
	}
}

void up_clicked(){
	u16 i,j,k,l;
	int to_move[26];
	int total_move = 0;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0){
			to_move[total_move++] = i;
		}
	}
	for(i=11;i<=12;i--){
		for(j=0;j<16;j++){
				for(k=0;k<total_move;k++){
					if(to_move[k] == current_level[i][j]-97){
						//execute moving sequence
						recursive_push(i,j,3);
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
						if(current_level[k][l] == objects[i]){
							current_level[k][l] = objects[j];
							updated[k][l] = 1;
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(overlap[i][j] != 95){
				GameStatus = 5;
				ScreenChange = 1;
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 5;
			ScreenChange = 1;
		}
	}
}

void down_clicked(){
	u16 i,j,k,l;
	int to_move[26];
	int total_move = 0;
	for(i=0;i<12;i++)
		for(j=0;j<16;j++)
			updated[i][j] = 0;
	if(GameStatus == 0) return;
	//check moving
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<24)) != 0){
			to_move[total_move++] = i;
		}
	}
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
				for(k=0;k<total_move;k++){
					if(to_move[k] == current_level[i][j]-97){
						//execute moving sequence
						recursive_push(i,j,4);
				}
			}
		}
	}
	
	parse_rules();
	//check replacement of things
	for(i=0;i<num_objects;i++){
		for(j=0;j<num_objects;j++){
			if(i!=j && (current_rules[objects[i]-97] & (1<<(objects[j]-97))) != 0){
				for(k=0;k<12;k++){
					for(l=0;l<16;l++){
						if(current_level[k][l] == objects[i]){
							current_level[k][l] = objects[j];
							updated[k][l] = 1;
						}
					}
				}
			}
		}
	}		
	board_update();
	//check win condition
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			if(overlap[i][j] != 95){
				GameStatus = 5;
				ScreenChange = 1;
			}
		}
	}
	for(i=0;i<26;i++){
		if((current_rules[i] & (1<<22)) != 0 && (current_rules[i] & (1<<24)) != 0){
			GameStatus = 5;
			ScreenChange = 1;
		}
	}
}

