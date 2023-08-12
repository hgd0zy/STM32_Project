#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"

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
	
	//USART1初始化配置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
}

//数据发送，在等待数据发送完成后返回
void Serial_SendByte(const uint8_t Data)
{
	USART_SendData(USART1, Data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

//发送一个汉字(utf-8)
void Serial_SendWord(const char *Word)
{
	Serial_SendByte(Word[0]);
	Serial_SendByte(Word[1]);
	Serial_SendByte(Word[2]);
}

//发送一个数组
void Serial_SendArray(const uint8_t *Array, uint16_t len)
{
	uint16_t i;
	for (i = 0; i < len; i++)
	{
		Serial_SendByte(Array[i]);
	}
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

//10的n次幂
uint32_t Serial_pow10(uint8_t n) {

  uint32_t result = 1;
  uint32_t base = 10;

  while (n) {
    if (n & 1)
      result *= base;
    base *= base;
    n >>= 1;
  }
  return result;
}

//发送一个数字
void Serial_SendNum(const uint32_t number, uint8_t length)
{
	while (length --)
	{
		Serial_SendByte((number / Serial_pow10(length) % 10) + '0');
	}
}

//重写printf函数底层的fputc
int fputc(int ch, FILE *file)
{
	Serial_SendByte(ch);
	return ch;
}

//封装printf函数
void Serial_printf(char *format, ...)
{
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	Serial_SendString(string);
	va_end(arg);
}
