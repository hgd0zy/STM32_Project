#include "stm32f10x.h"                  // Device header

#define dog 0

void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 配置ADC分频
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //工作在独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; // 选择启动ADC转换的通道数
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //定义外部触发源，此时没有使用外部触发源
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 连续或非连续模式，此时为非连续
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 选择扫描模式或非扫描模式，此时为非扫描
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);
	
	#if dog
	// 配置看门狗
	ADC_AnalogWatchdogThresholdsConfig(ADC1, 3500, 1000);
	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_0);
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	
	// 配置ADC中断为看门狗中断
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
	
	#endif
	
	// ADC状态校准
	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	// 软件启动AD转换
	// ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	//选择ADC转换通道与触发源
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_13Cycles5);
	// 软件启动AD转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	
	return ADC_GetConversionValue(ADC1);
}

/**
* @brief 配置ADC中断控制程序
  * @param 无
  * @retval 无
  */
void ADC_NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief 配置模拟看门狗中断处理程序
  * @param 无
  * @retval 无
  */
void ADC1_2_IRQHandler(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_ITConfig(ADC1,ADC_IT_AWD,DISABLE);
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_AWD) == SET)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
		ADC_ClearFlag(ADC1,ADC_FLAG_AWD);
		ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
	}
	ADC_ITConfig(ADC1,ADC_IT_AWD,ENABLE);
}
