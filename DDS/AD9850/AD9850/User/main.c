#include "stm32f10x.h" 
#include "AD9850.h"

int main(void)
{
		Init_AD9850();//ad9850≥ı ºªØ	
	  AD9850_Write_Serial(0,1000);
	while(1)
	{
	AD9850_Write_Serial(0,10000);
	}
}

