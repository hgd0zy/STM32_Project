#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define LED_light 0
#define water_light 0
#define GPIO_PINS uint16_t

GPIO_PINS GPIO_Pin[8] = {
	GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, 
	GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7
};

int main(void)
{
	//设置使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化输出模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//IO口实际操作
	GPIO_WriteBit(GPIOA, GPIO_Pin_All, Bit_SET);
	//GPIO_Write(GPIOA, GPIO_Pin_All); 与上一句等价，将所有口置为高电平
	
	while (1)
	{
		
		#if LED_light
		//LED闪烁
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		Delay_ms(500);
		
		#elif water_light
		//流水灯
		for(int i = 0;i<8;i++){
			GPIO_WriteBit(GPIOA, GPIO_Pin[i], Bit_RESET);
			Delay_ms(500);
			GPIO_WriteBit(GPIOA, GPIO_Pin[i], Bit_SET);
			Delay_ms(500);
		}
		
		#endif
	}
}
