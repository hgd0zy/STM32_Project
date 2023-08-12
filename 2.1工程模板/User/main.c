#include "stm32f10x.h"                  // Device header


int main(void)
{
	//寄存器方式
	//RCC->APB2ENR |= 0x00000010;
	//GPIOC->CRH |= 0x00300000;
	//GPIOC->ODR |= 0x00002000;
	
	//GPIO口使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	//设置IO口输出模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//实际操作
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	while(1){
		
	}
	
}
