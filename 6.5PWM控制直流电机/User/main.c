#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "key.h"
#include "Motor.h"

int8_t Speed = 0;

int main(void)
{
	OLED_Init();
	Key_Init();
	Motor_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{
		if (getKey_Num()) 
		{
			Speed += 20;
			if (Speed > 100) {Speed = -100;}
			Motor_SetSpeed(Speed);
		}
		OLED_ShowSignedNum(1, 7, Speed, 3);
	}
	//return 0;
}
