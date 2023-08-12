#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"

int main(void)
{
	uint8_t MPU6050_Adr, Test;
	int16_t MPU6050_ACCEL[3];
	int16_t MPU6050_GYRO[3];
	
	OLED_Init();
	MPU6050_Init();
	
	MPU6050_Adr = MPU6050_GetID();
	OLED_ShowString(1, 1, "ID:");
	OLED_ShowHexNum(1, 4, MPU6050_Adr, 2);
	
	Test = MPU6050_Test();
	OLED_ShowString(1, 8, "Test:");
	OLED_ShowHexNum(1, 14, Test, 2);
	
	while(1)
	{
		MPU6050_GetACCEL(MPU6050_ACCEL);
		MPU6050_GetGYRO(MPU6050_GYRO);
		
		OLED_ShowSignedNum(2, 1, MPU6050_ACCEL[0], 5);
		OLED_ShowSignedNum(3, 1, MPU6050_ACCEL[1], 5);
		OLED_ShowSignedNum(4, 1, MPU6050_ACCEL[2], 5);
		OLED_ShowSignedNum(2, 9, MPU6050_GYRO[0], 5);
		OLED_ShowSignedNum(3, 9, MPU6050_GYRO[1], 5);
		OLED_ShowSignedNum(4, 9, MPU6050_GYRO[2], 5);
	}
	//return 0;
}
