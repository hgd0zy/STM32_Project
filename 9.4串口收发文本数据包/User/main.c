#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include "string.h"

int main(void)
{
	Serial_Init();
	OLED_Init();
	LED_Init();
	
	OLED_ShowString(1, 1, "TX_DataPackage:");
	OLED_ShowString(3, 1, "RX_DataPackage:");
	
	Serial_SendString("Hello,World!\r\n");
	OLED_ShowString(2, 1, "Hello,World!");
	
	while(1)
	{
		if (Serial_GetFlag())
		{
			if (strcmp(RX_DataPackage, "LED_ON") == 0)
			{
				LED_ON(0);
				Serial_SendString("LED_ON_OK\r\n");
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(4, 1, "LED_ON_OK");
			}
			else if (strcmp(RX_DataPackage, "LED_OFF") == 0) 
			{
				LED_OFF(0);
				Serial_SendString("LED_OFF_OK\r\n");
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(4, 1, "LED_OFF_OK");
			}
			else 
			{
				Serial_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(4, 1, "                ");
				OLED_ShowString(4, 1, "ERROR_COMMAND");
			}
			Serial_ClearFlag();
		}
	}
	//return 0;
}

