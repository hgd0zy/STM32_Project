#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include "Key.h"

int main(void)
{
	Serial_Init();
	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "TX_DataPackage:");
	OLED_ShowString(3, 1, "RX_DataPackage:");
	
	TX_DataPackage[0] = 0x01;
	TX_DataPackage[1] = 0x02;
	TX_DataPackage[2] = 0x03;
	TX_DataPackage[3] = 0x04;
	
	while(1)
	{
		if (getKey_Num() == 1) 
		{
			Serial_SendPackage();
			
			OLED_ShowHexNum(2, 1, TX_DataPackage[0], 2);
			OLED_ShowHexNum(2, 4, TX_DataPackage[1], 2);
			OLED_ShowHexNum(2, 7, TX_DataPackage[2], 2);
			OLED_ShowHexNum(2, 10, TX_DataPackage[3], 2);
			
			TX_DataPackage[0] ++;
			TX_DataPackage[1] ++;
			TX_DataPackage[2] ++;
		    TX_DataPackage[3] ++;
		}
		
		if (Serial_GetFlag())
		{
			OLED_ShowHexNum(4, 1, RX_DataPackage[0], 2);
			OLED_ShowHexNum(4, 4, RX_DataPackage[1], 2);
			OLED_ShowHexNum(4, 7, RX_DataPackage[2], 2);
			OLED_ShowHexNum(4, 10, RX_DataPackage[3], 2);
		}
	}
	//return 0;
}

