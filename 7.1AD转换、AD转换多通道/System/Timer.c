#include "stm32f10x.h"                  // Device header

void Timer_Init(void){
	//使能APB1时钟，TIM2在APB1总线上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//定时器内部时钟设置
	TIM_InternalClockConfig(TIM2);
	
	//配置定时器运行控制
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStructure.TIM_Period = 10000 - 1;
	TIM_InitStructure.TIM_Prescaler = 7200 - 1;
	TIM_InitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	//配置更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//配置中断控制器分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM2, ENABLE);
}

/*
void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
