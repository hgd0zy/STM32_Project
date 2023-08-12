#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_Address (0x68 << 1)

//事件超时
static void WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Time = 10000;
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Time --;
		if (Time == 0) {return ;}
	}
}

//指定MPU6050寄存器写入值
void MPU6050_WReg(uint8_t Reg, uint8_t Command)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, Reg);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C1, Command);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
}

//获取MPU6050指定寄存器值
uint8_t MPU6050_RReg(uint8_t Reg)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, Reg);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Receiver);
	WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	return Data;
}

//初始化MPU6050电源与寄存器配置等
void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	I2C_Cmd(I2C1, ENABLE);
	
	MPU6050_WReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WReg(MPU6050_CONFIG, 0x06);
	MPU6050_WReg(MPU6050_ACCEL_CONFIG, 0x18);
	MPU6050_WReg(MPU6050_GYRO_CONFIG, 0x18);
}

//获取MPU模块总线地址
uint8_t MPU6050_GetID(void)
{
	return MPU6050_RReg(MPU6050_WHO_AM_I);
}

//发送一个数组的数据，即连续写
void MPU6050_WriteRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length)
{
	uint8_t i;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, Reg);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	for	(i=0;i<Length - 1;i++) 
	{
		I2C_SendData(I2C1, Array[i]);
		WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	}
	
	I2C_SendData(I2C1, Array[i]);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
}

//接收一个数组的数据，即连续读
void MPU6050_ReadRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length)
{
	uint8_t i;
	
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C1, Reg);
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C1, ENABLE);
	WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C1, MPU6050_Address, I2C_Direction_Receiver);
	WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	for	(i=0;i<Length - 1;i++) 
	{
		WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
		Array[i] =I2C_ReceiveData(I2C1);
	}
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Array[i] =I2C_ReceiveData(I2C1);
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
}

//获取加速度计寄存器值
void MPU6050_GetACCEL(int16_t *Array)
{
	uint8_t Accel[6];
	
	MPU6050_ReadRegArray(Accel, MPU6050_ACCEL_XOUT_H, 6);
	Array[0] = (Accel[0] << 8) | Accel[1];
	Array[1] = (Accel[2] << 8) | Accel[3];
	Array[2] = (Accel[4] << 8) | Accel[5];
}

//获取角速度计寄存器值
void MPU6050_GetGYRO(int16_t *Array)
{
	uint8_t Gyro[6];
	
	MPU6050_ReadRegArray(Gyro, MPU6050_GYRO_XOUT_H, 6);
	Array[0] = (Gyro[0] << 8) | Gyro[1];
	Array[1] = (Gyro[2] << 8) | Gyro[3];
	Array[2] = (Gyro[4] << 8) | Gyro[5];
}

uint8_t MPU6050_Test(void)
{
	uint8_t TestArray[2] = {0xEE, 0xFF};
	
	MPU6050_WriteRegArray(TestArray, MPU6050_INT_PIN_CFG, 2);
	return MPU6050_RReg(MPU6050_INT_ENABLE);
}
