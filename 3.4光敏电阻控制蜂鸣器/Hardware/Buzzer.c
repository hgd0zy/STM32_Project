#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
  * @brief 初始化蜂鸣器，在操作蜂鸣器之前调用
  * @param 无
  * @retval 无
  */
void Buzzer_Init(void)
{
	//使能蜂鸣器输出口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//初始化输出口,设置输出速度和模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

/**
  * @brief 启动蜂鸣器
  * @param 无
  * @retval 无
  */
void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

/**
  * @brief 关闭蜂鸣器
  * @param 无
  * @retval 无
  */
void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
