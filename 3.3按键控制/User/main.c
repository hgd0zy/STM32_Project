#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "key.h"
#include "Delay.h"

#define LEDtest 0
#define KeyLED 1

int main(void)
{
	uint8_t key = 0;
	
	LED_Init(); // 初始化LED，使其初始为关并可以点亮
	Key_Init(); // 初始化按键，使其可以识别到按下
	
	while(1)
	{
		#if LEDtest
		
		LED_ON(LED[1]);
		Delay_ms(500);
		LED_ON(LED[2]);
		Delay_ms(500);
		
		LED_OFF(LED[1]);
		Delay_ms(500);
		LED_OFF(LED[2]);
		Delay_ms(500);
		
		#elif KeyLED
		
		key = getKey_Num();
		if(key)
		{
			LED_Turn(LED[key]);
		}
		#endif
	}
	//return 0;
}
