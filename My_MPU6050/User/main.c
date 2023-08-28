#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"
#include "Motor.h"
#include "Serial.h"
#include "EXTIT.h"
#include "Timer.h"

uint8_t MPU6050_Adr;
int16_t MPU6050_ACCEL[3];
int16_t MPU6050_GYRO[3];
float pitch_acc, roll_acc;
float pitch_gyro, roll_gyro, yaw;
float pitch_k, roll_k;

int main(void)
{
	
	OLED_Init();
	MPU6050_Init();
	Motor_Init();
	Serial_Init();
	Timer_Init();
	EXTIT_Init();
	
	MPU6050_Adr = MPU6050_GetID();
	OLED_ShowString(1, 1, "ID:");
	OLED_ShowHexNum(1, 4, MPU6050_Adr, 2);
	
	while(1)
	{
//		if(Serial_GetFlag())
//		{
//			switch (Serial_GetRX())
//			{
//				case 'w':
//					Motor_SetSpeed(90);
//					break;
//				case 'x':
//					Motor_SetSpeed(0);
//					break;
//				default:
//					break;
//			}
//		}
//		
		Serial_printf("%f,%f\n", pitch_acc, pitch_k);
		
		OLED_ShowSignedNum(2, 1, pitch_acc, 3);
		OLED_ShowSignedNum(3, 1, roll_acc, 3);
		
		OLED_ShowSignedNum(2, 6, pitch_k, 5);
		OLED_ShowSignedNum(3, 6, roll_k, 5);
		OLED_ShowSignedNum(4, 6, yaw, 5);
		
//		OLED_ShowSignedNum(2, 1, MPU6050_ACCEL[0], 5);
//		OLED_ShowSignedNum(3, 1, MPU6050_ACCEL[1], 5);
//		OLED_ShowSignedNum(4, 1, MPU6050_ACCEL[2], 5);
//		OLED_ShowSignedNum(2, 9, MPU6050_GYRO[0], 5);
//		OLED_ShowSignedNum(3, 9, MPU6050_GYRO[1], 5);
//		OLED_ShowSignedNum(4, 9, MPU6050_GYRO[2], 5);
	}
	//return 0;
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
//		MPU6050_GetACCEL(MPU6050_ACCEL);
//		MPU6050_GetGYRO(MPU6050_GYRO);
//		calculate_acc(&pitch_acc, &roll_acc);
//		calculate_gyro(&pitch_gyro, &roll_gyro, &yaw);
		
		kalman_filter(&pitch_k, &roll_k);
		
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
