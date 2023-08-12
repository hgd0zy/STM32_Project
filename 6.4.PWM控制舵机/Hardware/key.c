#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void Key_Init(void)
{
	//GPIOB使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//初始化按键所用GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//得到按下的按键数字
uint8_t getKey_Num(void)
{
	uint8_t keynum = 0;
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == RESET)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
		Delay_ms(20);
		keynum = 1;
	}
	
	return keynum;
}
