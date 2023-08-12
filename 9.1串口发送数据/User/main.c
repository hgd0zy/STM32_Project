#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	Serial_Init();
	
	Serial_SendByte('A');
	
	uint8_t a[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(a, 4);
	
	char *string = "\r\nHelloWorld!";
	Serial_SendString(string);
	
	char *word = "\r\n你好,世界!\r\n";
	Serial_SendString(word);
	
	Serial_SendNum(12345, 5);
	
	//第一种printf移植方式为重写fputc函数，缺点是只能在一个串口上输出
	printf("\r\nNum1=%d", 111);
	
	//第二种printf移植方式为先转移字符串再调用串口输出，缺点是重复编写
	char String[20];
	sprintf(String, "\r\nNum2=%d", 222);
	Serial_SendString(String);
	
	//第三种printf移植方式是可变参数封装，是第二种方式的封装，更加方便
	Serial_printf("\r\nNum3=%d Num4=%d\r\n", 333, 444);
	
	while(1)
	{
		
	}
	//return 0;
}
