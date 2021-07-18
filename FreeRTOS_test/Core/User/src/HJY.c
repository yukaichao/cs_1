#include "main.h"
#include "oled.h"
#include "math.h"

void display_world_of_wonder(uint8_t x0,uint8_t y0,uint8_t r)
{
	uint8_t x,y;
	for(x = 0;x <= 63;x++){
		y = sqrt(pow(r,2)-pow(x-x0,2))+y0; //圆方程  x,y反置
//		OLED_DrawPoint(y,x,1);      //上半圆
//		OLED_DrawPoint(63-y,x,1);   //下半圆
	}

}
