#include "stm32f10x.h"                  // Device header

/**
  * @brief 初始化光敏传感器，在使用光敏传感器前调用
  * @param 无
  * @retval 无
  */
void LDR_Init(void)
{
	//使能光敏传感器输入口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//初始化输入口,设置输入速度和模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief 得到光敏传感器的输出
  * @param 无
  * @retval 光敏传感器的输出(IO口输入)
  */
uint8_t getLDR(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
}
