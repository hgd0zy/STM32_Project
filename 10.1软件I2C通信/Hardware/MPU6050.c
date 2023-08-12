#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "MyI2C.h"

#define MPU6050_Address (0x68 << 1)

//指定MPU6050寄存器写入值
void MPU6050_WReg(uint8_t Reg, uint8_t Command)
{
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x00); //写命令的最后一位为0
	MyI2C_ReceiveACK();
	MyI2C_SendByte(Reg);					//寄存器地址
	MyI2C_ReceiveACK();
	MyI2C_SendByte(Command);				//写入寄存器的值
	MyI2C_ReceiveACK();
	MyI2C_STOP();
}

//获取MPU6050指定寄存器值
uint8_t MPU6050_RReg(uint8_t Reg)
{
	uint8_t Data;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x00); //写命令的最后一位为0
	MyI2C_ReceiveACK();
	MyI2C_SendByte(Reg);					//寄存器地址
	MyI2C_ReceiveACK();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x01); //读命令的最后一位为1
	MyI2C_ReceiveACK();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendACK(1);
	MyI2C_STOP();
	
	return Data;
}

//初始化MPU6050电源与寄存器配置等
void MPU6050_Init(void)
{
	MyI2C_Init();
	
	MPU6050_WReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WReg(MPU6050_CONFIG, 0x06);
	MPU6050_WReg(MPU6050_ACCEL_CONFIG, 0x18);
	MPU6050_WReg(MPU6050_GYRO_CONFIG, 0x18);
}

//获取MPU6050总线地址
uint8_t MPU6050_GetID(void)
{
	return MPU6050_RReg(MPU6050_WHO_AM_I);
}

//发送一个数组的数据，即连续写
void MPU6050_WriteRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length)
{
	uint8_t i;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x00); //写命令的最后一位为0
	MyI2C_ReceiveACK();
	MyI2C_SendByte(Reg);					//寄存器地址
	MyI2C_ReceiveACK();
	
	for	(i=0;i<Length;i++) 
	{
		MyI2C_SendByte(Array[i]);
		MyI2C_ReceiveACK();//接收应答
	}
	MyI2C_STOP();
}

//接收一个数组的数据，即连续读
void MPU6050_ReadRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length)
{
	uint8_t i;
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x00); //写命令的最后一位为0
	MyI2C_ReceiveACK();
	MyI2C_SendByte(Reg);					//寄存器地址
	MyI2C_ReceiveACK();
	
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_Address | 0x01); //读命令的最后一位为1
	MyI2C_ReceiveACK();
	for	(i=0;i<Length - 1;i++) 
	{
		Array[i] = MyI2C_ReceiveByte();
		MyI2C_SendACK(0);//发送应答
	}
	Array[i] = MyI2C_ReceiveByte();
	MyI2C_SendACK(1);//发送非应答
	MyI2C_STOP();
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
