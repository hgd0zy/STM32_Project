#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Delay.h"

int main(void)
{
	int16_t Num = 0;
	
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1, 1, "Num:");
	
	while(1)
	{
		Delay_ms(20);
		Num += getEncoderCount();
		OLED_ShowSignedNum(1, 5, Num, 5);
		Delay_ms(20);
	}
	//return 0;
}
