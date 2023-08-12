#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	IC_Init();
	PWM_Init();
	
	
	PWM_SetPrescaler(720 - 1);  // Freq = 72M/(PSC-1)/(ARR-1)
	PWM_SetCompare1(60); 		// Duty = Compare/ARR
	
	OLED_ShowString(1, 1, "Freq:     Hz");
	OLED_ShowString(2, 1, "Duty:  %");
	
	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(2, 6, IC_GetDuty(), 2);
	}
	//return 0;
}
