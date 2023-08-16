#include "stm32f10x.h"                  // Device header

/**
  * @brief 初始化TIM2来输出PWM波形
  * @param 无
  * @retval 无
  */
void PWM_Init(void){
	//使能APB2时钟，TIM1在APB2总线上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//使能GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//定时器内部时钟设置
	TIM_InternalClockConfig(TIM1);
		
	//配置PA8
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 在使用片上外设作为输出时需配置为复用推挽输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//配置定时器运行控制
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 7200 - 1; //ARR
	TIM_InitStructure.TIM_Prescaler = 1 - 1; //PSC
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_InitStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	
	//配置输出比较模式
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_OCInitTypeDef TIMOC_InitStructure;
	TIM_OCStructInit(&TIMOC_InitStructure);
	TIMOC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1; //使用PWM1模式，ARR小于CCR时为有效电平，大于等于时为无效电平
	TIMOC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置高电平为有效电平
	TIMOC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_InitStructure.TIM_Pulse = 0; //CCR
	
	TIM_OC1Init(TIM1, &TIMOC_InitStructure);
	TIM_OC2Init(TIM1, &TIMOC_InitStructure);
	TIM_Cmd(TIM1, ENABLE);
}

/**
  * @brief 设置脉冲宽度，越小高电平时间越短
  * @param 宽度值
  * @retval 无
  */
void PWM_SetCompare1(uint16_t Compare1){
	TIM_SetCompare1(TIM1, Compare1);
}

void PWM_SetCompare2(uint16_t Compare2)
{
	TIM_SetCompare2(TIM1, Compare2);
}
