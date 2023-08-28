#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Motor.h"
#include "Serial.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "PID.h"
#include "EXTIT.h"

int8_t Speed = 0;
char Command;
int16_t max  = -32768, min = 32767;
uint16_t l_speed, r_speed;
static float k_pitch, k_roll;

int main(void)
{
	OLED_Init();
	Serial_Init();
	MPU6050_Init();
	Motor_Init();
	Encoder_Init();
	PID_Init();
	EXTIT_Init();
	
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
					if (Speed < 55) {Speed += 5;}
					Motor_SetSpeed(Speed);
					break;
				case 's':
					Speed = -15;
					Motor_SetSpeed(Speed);
					break;
				case 'x':
					if(Speed > 0) {Speed -= 5;}
					else if(Speed < 0){Speed += 5;}
					Motor_SetSpeed(Speed);
					break;
				default:
					Serial_printf("Error Command!\r\n");
					break;
			}
		}
		
		OLED_ShowSignedNum(1, 1, k_pitch, 3);
		OLED_ShowSignedNum(2, 1, k_roll, 3);
	}
	//return 0;
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		kalman_filter(&k_pitch, &k_roll);
		
		PID_Ctrl(l_speed, r_speed);
		
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{	
		l_speed = Encoder_Get1();
		r_speed = Encoder_Get2();
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
