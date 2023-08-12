#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue0, ADValue1, ADValue2, ADValue3;

int main(void)
{	
	OLED_Init();
	AD_Init();
	
	OLED_ShowString(1, 1, "AD0:");
	OLED_ShowString(2, 1, "AD1:");
	OLED_ShowString(3, 1, "AD2:");
	OLED_ShowString(4, 1, "AD3:");
	
	while(1)
	{
		ADValue0 = AD_GetValue(ADC_Channel_0);
		ADValue1 = AD_GetValue(ADC_Channel_1);
		ADValue2 = AD_GetValue(ADC_Channel_2);
		ADValue3 = AD_GetValue(ADC_Channel_3);

		OLED_ShowNum(1, 5, ADValue0, 4);
		OLED_ShowNum(2, 5, ADValue1, 4);
		OLED_ShowNum(3, 5, ADValue2, 4);
		OLED_ShowNum(4, 5, ADValue3, 4);
		
		Delay_ms(100);
	}
}
