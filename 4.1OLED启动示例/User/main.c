#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "Hello World!");
	OLED_ShowNum(2, 1, 123, 3);
	OLED_ShowSignedNum(2, 5, -66, 2);
	OLED_ShowHexNum(2, 9, 0xAABB, 4);
	OLED_ShowBinNum(3, 1, 0x3090, 16);
	//OLED_Clear();
	
	while(1)
	{
		
	}
	//return 0;
}
