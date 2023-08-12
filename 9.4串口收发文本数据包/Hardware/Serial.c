#include "stm32f10x.h"                  // Device header

uint8_t RX_Data;
uint8_t RX_Flag = 0;
char RX_DataPackage[100];

void Serial_Init(void)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//GPIO口初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1初始化配置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);

	//串口中断配置
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//嵌套向量中断控制器
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

//数据发送，在等待数据发送完成后返回
void Serial_SendByte(const uint8_t Data)
{
	USART_SendData(USART1, Data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

//发送一个字符串
void Serial_SendString(const char *String)
{
	uint16_t i = 0;
	while (String[i] != '\0')
	{
		Serial_SendByte(String[i++]);
	}
}

//获取数据接收标志
uint8_t Serial_GetFlag(void)
{
	if (RX_Flag == 0)
	{
		return 0;
	}
	return 1;
}

//设置为可接收状态
void Serial_ClearFlag(void)
{
	RX_Flag = 0;
}

//串口中断处理函数
void USART1_IRQHandler(void)
{
	static uint8_t RX_STATE = 0;
	static uint8_t i = 0;
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		RX_Data = USART_ReceiveData(USART1);
		
		if (RX_Flag != 0) {RX_STATE = 3;}
		
		switch(RX_STATE)
		{
			case 0:
				if (RX_Data == '#') {RX_STATE = 1;}
				break;
			case 1:
				if	(RX_Data != '\r') { RX_DataPackage[i++] = RX_Data; }
				else { RX_STATE = 2; }
				break;
			case 2:
				if (RX_Data == '\n') 
				{
					RX_Flag = 1;
					RX_STATE = 0;
					RX_DataPackage[i] = '\0';
					i = 0;
				}
				break;
			default:
				if (RX_Flag == 0) {RX_STATE = 0;}
				break;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
