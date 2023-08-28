#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "math.h"

#define MPU6050_Address (0x68 << 1)
#define PI 3.1415926535897

static int16_t MPU6050_ACCEL[3];
static int16_t MPU6050_GYRO[3];
static float pitch_a, roll_a;
static float pitch_g, roll_g;
static float GYRO_Motrix[3][3] = {{1, 0, 0},{0, 1, 0},{0, 0, 1}};

static float k_roll = 0, k_pitch = 0;
static float P[2][2] = {{1, 0},{0, 1}}, P_[2][2], K[2][2];
static float Q[2][2] = {{0.025, 0},{0, 0.025}};
static float R[2][2] = {{0.3, 0},{0, 0.3}};

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
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, Reg);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2, Command);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
	
}

//获取MPU6050指定寄存器值
uint8_t MPU6050_RReg(uint8_t Reg)
{
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, Reg);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Receiver);
	WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Data = I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
}

//初始化MPU6050电源与寄存器配置等
void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WReg(MPU6050_ACCEL_CONFIG, 0x07);
	MPU6050_WReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WReg(MPU6050_CONFIG, 0x16);
	MPU6050_WReg(MPU6050_SMPLRT_DIV, 0x13);//50HZ
	MPU6050_WReg(MPU6050_INT_PIN_CFG, 0x80);//配置低电平中断
	MPU6050_WReg(MPU6050_INT_ENABLE, 0x01);//中断使能
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
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, Reg);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	for	(i=0;i<Length - 1;i++) 
	{
		I2C_SendData(I2C2, Array[i]);
		WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	}
	
	I2C_SendData(I2C2, Array[i]);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}

//接收一个数组的数据，即连续读
void MPU6050_ReadRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length)
{
	uint8_t i;
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Transmitter);
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2, Reg);
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	I2C_GenerateSTART(I2C2, ENABLE);
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2, MPU6050_Address, I2C_Direction_Receiver);
	WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	
	for	(i=0;i<Length - 1;i++) 
	{
		WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
		Array[i] =I2C_ReceiveData(I2C2);
	}
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	Array[i] =I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
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

//使用加速度计解算姿态
void calculate_acc(float *pitch, float *roll)
{
	MPU6050_GetACCEL(MPU6050_ACCEL);
	
	float tan_alpha = (float)MPU6050_ACCEL[1] / (float)MPU6050_ACCEL[2];
	float tan_beta = MPU6050_ACCEL[1] * MPU6050_ACCEL[1] + MPU6050_ACCEL[2] * MPU6050_ACCEL[2];
	tan_beta = sqrtf(tan_beta);
	tan_beta = MPU6050_ACCEL[0] / tan_beta;
	
	*pitch = -atanf(tan_beta) / PI * 180;
	*roll = atanf(tan_alpha) / PI * 180;
	//*yaw = MPU6050_ACCEL[2] ;
}

//使用角速度计解算姿态
void calculate_gyro(float *pitch, float *roll, float *yaw)
{
	static float V_pitch, V_roll, V_yaw;
	
	MPU6050_GetGYRO(MPU6050_GYRO);
	
	GYRO_Motrix[0][1] = (sinf(*pitch) * sinf(*roll)) / cosf(*pitch);
	GYRO_Motrix[0][2] = (cosf(*roll) * sinf(*pitch)) / cosf(*pitch);
	GYRO_Motrix[1][1] = cosf(*roll);
	GYRO_Motrix[1][2] = -sinf(*roll);
	GYRO_Motrix[2][1] = sinf(*roll) / cosf(*pitch);
	GYRO_Motrix[2][2] = cosf(*roll) / cosf(*pitch);
	V_roll = (GYRO_Motrix[0][0]*MPU6050_GYRO[0] + GYRO_Motrix[0][1]*MPU6050_GYRO[1] + GYRO_Motrix[0][2]*MPU6050_GYRO[2]) / 16.4;
	V_pitch = (GYRO_Motrix[1][0]*MPU6050_GYRO[0] + GYRO_Motrix[1][1]*MPU6050_GYRO[1] + GYRO_Motrix[1][2]*MPU6050_GYRO[2]) / 16.4;
	V_yaw = (GYRO_Motrix[2][0]*MPU6050_GYRO[0] + GYRO_Motrix[2][1]*MPU6050_GYRO[1] + GYRO_Motrix[2][2]*MPU6050_GYRO[2]) / 16.4;
	
	*pitch = *pitch + V_pitch * 0.02;
	*roll = *roll + V_roll * 0.02;
	*yaw = *yaw + V_yaw * 0.02;
}

//卡尔曼滤波
void kalman_filter(float *pitch, float *roll)
{
	static float V_pitch, V_roll;
	
	calculate_acc(&pitch_a, &roll_a);
	MPU6050_GetGYRO(MPU6050_GYRO);
	
	MPU6050_GYRO[0] -= (-42);
	MPU6050_GYRO[1] -= (-26);
	MPU6050_GYRO[2] -= (-99);
	
	GYRO_Motrix[0][1] = (sinf(*pitch) * sinf(*roll)) / cosf(*pitch);
	GYRO_Motrix[0][2] = (cosf(*roll) * sinf(*pitch)) / cosf(*pitch);
	GYRO_Motrix[1][1] = cosf(*roll);
	GYRO_Motrix[1][2] = -sinf(*roll);
	GYRO_Motrix[2][1] = sinf(*roll) / cosf(*pitch);
	GYRO_Motrix[2][2] = cosf(*roll) / cosf(*pitch);
	V_roll = (GYRO_Motrix[0][0]*MPU6050_GYRO[0] + GYRO_Motrix[0][1]*MPU6050_GYRO[1] + GYRO_Motrix[0][2]*MPU6050_GYRO[2]) / 16.4;
	V_pitch = (GYRO_Motrix[1][0]*MPU6050_GYRO[0] + GYRO_Motrix[1][1]*MPU6050_GYRO[1] + GYRO_Motrix[1][2]*MPU6050_GYRO[2]) / 16.4;
	
	//Step1
	pitch_g = k_pitch + V_pitch * 0.02;
	roll_g = k_roll + V_roll * 0.02;
	
	//Step2
	P_[0][0] = P[0][0] + Q[0][0];
	P_[0][1] = 0;
	P_[1][0] = 0;
	P_[1][1] = P[1][1] + Q[1][1];
	
	//Step3
	K[0][0] = P_[0][0]/(P_[0][0] + R[0][0]);
	K[0][1] = 0;
	K[1][0] = 0;
	K[1][1] = P_[1][1]/(P_[1][1] + R[1][1]);
	
	//Step4
	k_roll = roll_g + K[0][0]*(roll_a - roll_g);
	k_pitch = pitch_g + K[1][1]*(pitch_a - pitch_g);
	
	//Step5
	P[0][0] = (1 - K[0][0])*P[0][0];
	P[0][1] = 0;
	P[1][0] = 0;
	P[1][1] = (1 - K[1][1])*P[1][1];
	
	*pitch = k_pitch;
	*roll = k_roll;
}

//测试用
uint8_t MPU6050_Test(void)
{
	uint8_t TestArray[2] = {0xEE, 0xFF};
	
	MPU6050_WriteRegArray(TestArray, MPU6050_INT_PIN_CFG, 2);
	return MPU6050_RReg(MPU6050_INT_ENABLE);
}
