#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Motor.h"
#include "Serial.h"
#include "Encoder.h"
#include "MPU6050.h"

int8_t Speed = 0;
char Command;
int16_t max  = -32768, min = 32767;
int16_t MPU6050_ACCEL[3];
int16_t MPU6050_GYRO[3];

int main(void)
{
	OLED_Init();
	Motor_Init();
	Serial_Init();
	Encoder_Init();
	MPU6050_Init();
	
	OLED_ShowString(1, 1, "V:");
	//OLED_ShowString(2, 1, "HC-05:");
	
	while(1)
	{
		if (Serial_GetFlag())
		{
			Command = Serial_GetRX();
			switch (Command)
			{
				case 'a':
					break;
				case 'd':
					break;
				case 'w':
					if (Speed < 100) {Speed += 20;}
					Motor_SetSpeed(Speed);
					break;
				case 's':
					Speed = -40;
					Motor_SetSpeed(Speed);
					break;
				case 'x':
					if(Speed > 0) {Speed -= 20;}
					else if(Speed < 0){Speed += 20;}
					Motor_SetSpeed(Speed);
					break;
				default:
					Serial_printf("Error Command!\r\n");
					break;
			}
		}
		OLED_ShowSignedNum(1, 3, Speed, 3);
		//OLED_ShowChar(2, 7, Command);
		OLED_ShowSignedNum(1, 9, (int16_t)Encoder_Get(), 5);
		
		MPU6050_GetACCEL(MPU6050_ACCEL);
		MPU6050_GetGYRO(MPU6050_GYRO);
		
		OLED_ShowSignedNum(2, 1, MPU6050_ACCEL[0], 5);
		OLED_ShowSignedNum(3, 1, MPU6050_ACCEL[1], 5);
		OLED_ShowSignedNum(4, 1, MPU6050_ACCEL[2], 5);
		OLED_ShowSignedNum(2, 9, MPU6050_GYRO[0], 5);
		OLED_ShowSignedNum(3, 9, MPU6050_GYRO[1], 5);
		OLED_ShowSignedNum(4, 9, MPU6050_GYRO[2], 5);
		
		if (MPU6050_ACCEL[2] > max) {max = MPU6050_ACCEL[2];}
		else if (MPU6050_ACCEL[2]<min) {min = MPU6050_ACCEL[2];}
		//Serial_printf("%d %d  ", max, min);
		//Serial_printf("%.4f\t", (float)(MPU6050_ACCEL[0]-1069)/16384*1.2*180/3.14/9.8);
	}
	//return 0;
}
