#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * @brief 初始化直流电机驱动
  * @param 无
  * @retval 无
  */
void Motor_Init(void){
	//使能GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置PA4 PA5
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
}

/**
  * @brief 设置电机转速
  * @param 转速
  * @retval 无
  */
void Motor_SetSpeed(int8_t Speed){
	if (Speed>=0) 
	{
		//设置电机转动方向
		GPIO_SetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_1);
		//设置转速
		PWM_SetCompare1(Speed);
		PWM_SetCompare2(Speed);
	}
	else 
	{
		//设置电机转动方向
		GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_0);
		GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_1);
		//设置转速
		PWM_SetCompare1(-Speed);
		PWM_SetCompare2(-Speed);
	}
}
