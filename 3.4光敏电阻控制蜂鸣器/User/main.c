#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"
#include "Delay.h"
#include "LDR.h"

#define BuzzerTest 0
#define LightBuzzer 1

int main(void)
{
	Buzzer_Init();
	while(1)
	{
		#if BuzzerTest
		
		Buzzer_ON();
		Delay_ms(500);
		Buzzer_OFF();
		Delay_ms(500);
		
		#elif LightBuzzer
		
		if(getLDR() == 0)
		{
			Buzzer_ON();
		}
		else 
		{
			Buzzer_OFF();
		}
		#endif
	}
	//return 0;
}
