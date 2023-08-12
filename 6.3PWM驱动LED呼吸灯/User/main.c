#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

uint8_t direction1 = 1, Compare1 = 1;

int main(void)
{
	PWM_Init();
	while(1)
	{
		if(Compare1 >= 100 || Compare1 <= 0){ direction1 = -direction1; }
		Compare1+=direction1;
		PWM_SetCompare1(Compare1);
		Delay_ms(10);
	}
	//return 0;
}
