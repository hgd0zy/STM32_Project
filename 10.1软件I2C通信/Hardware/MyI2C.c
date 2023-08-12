#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define SCL_Set(x) (GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x)), Delay_us(10))
#define SDA_Set(x) (GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x)), Delay_us(10))
#define Read_SDA() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

//I2C输出口初始化
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
	
}

//I2C通信开始
void MyI2C_Start(void)
{
	SDA_Set(1);
	SCL_Set(1);
	SDA_Set(0);
	SCL_Set(0);
}

//I2C通信结束
void MyI2C_STOP(void)
{
	SDA_Set(0);
	SCL_Set(1);
	SDA_Set(1);
}

//发送一个字节
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	
	for (i=0;i<8;i++)
	{
		SDA_Set(Byte & (0x80 >> i ));
		SCL_Set(1);
		SCL_Set(0);
	}
}

//接收一个字节
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Data = 0x00;
	uint8_t i;
	
	SDA_Set(1);
	
	for (i=0;i<8;i++)
	{
		SCL_Set(1);
		Data = Data << 1;
		Data |= Read_SDA();
		SCL_Set(0);
	}
	return Data;
}

//发送应答位
void MyI2C_SendACK(uint8_t ACKbit)
{
	SDA_Set(ACKbit);
	SCL_Set(1);
	SCL_Set(0);
}

//接收应答位
uint8_t MyI2C_ReceiveACK(void)
{
	uint8_t ACKbit;
	
	SDA_Set(1);
	SCL_Set(1);
	ACKbit = Read_SDA();
	SCL_Set(0);
	
	return ACKbit;
}
