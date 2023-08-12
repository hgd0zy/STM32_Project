#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Servo.h"
#include "key.h"

uint16_t Angle = 0;

int main(void)
{
	OLED_Init();
	Servo_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "Angle:");
	
	while(1)
	{
		if(getKey_Num() == 1)
		{
			Angle += 30;
			if(Angle > 180){
				Angle = 0;
			}
			Servo_SetAngle(Angle);
		}
		OLED_ShowNum(1, 7, Angle,3);
	}
	//return 0;
}
