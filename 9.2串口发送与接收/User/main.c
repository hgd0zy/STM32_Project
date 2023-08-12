#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

uint8_t RXData;

int main(void)
{
	Serial_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "RXData:");
	
	while(1)
	{
		if (Serial_GetFlag())
		{
			RXData = Serial_GetRX();
			OLED_ShowHexNum(1, 8, RXData, 2);
			Serial_SendByte(RXData);
		}
	}
	//return 0;
}
