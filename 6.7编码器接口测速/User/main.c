#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"
#include "Timer.h"

int16_t Speed = 0;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
	}
	//return 0;
}

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		Speed = (int16_t)(Encoder_Get());
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
