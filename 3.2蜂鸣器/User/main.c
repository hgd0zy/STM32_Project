#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Buzzer 0

int main(void)
{
	//使能GPIOB口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//设置GPIOB口为推挽输出模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//实际操作
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	
	while(1){
		#if Buzzer
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
		Delay_ms(100);
		GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
		Delay_ms(500);
		#endif
	}
}
