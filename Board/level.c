#include "stm32f10x.h"
#include "level.h"
#include "level_design.h"
#include "IERG3810_TFTLCD.h"

void level_init(u8 lvl){
	u8 i,j;
	int index;
	IERG3810_TFTLCD_FillRectangle(0x0,0,320,0,240);
	if(lvl>4) return;
	for(i=0;i<12;i++){
		for(j=0;j<16;j++){
			index = mapping[levels[lvl][i][j]-65];
			if(index >= 0)
				IERG3810_TFTLCD_ShowImage(j*20,(11-i)*20,index);
		}
	}
}

