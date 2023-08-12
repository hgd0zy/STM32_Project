#include "stm32f10x.h"                  // Device header

#define GPIO_PINS uint16_t

static GPIO_PINS GPIO_Pins[2] = {GPIO_Pin_0, GPIO_Pin_1};


//初始化LED灯口
void LED_Init(void){
	//IO口使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//设置IO口输出模式和速度
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//初始化为关闭状态
	GPIO_SetBits(GPIOA, GPIO_Pin_All);
}


//点亮某个LED灯
void LED_ON(uint8_t num)
{
	GPIO_ResetBits(GPIOA, GPIO_Pins[num]);
}


//关闭某个LED灯
void LED_OFF(uint8_t num)
{
	GPIO_SetBits(GPIOA, GPIO_Pins[num]);
}


//使某个LED灯状态取反
void LED_Turn(uint8_t num)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pins[num]) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pins[num]);
	}
	else 
	{
		GPIO_ResetBits(GPIOA, GPIO_Pins[num]);
	}
}
