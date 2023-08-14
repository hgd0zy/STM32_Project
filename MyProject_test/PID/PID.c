#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "PWM.h"
#include "Encoder.h"
#include "Timer.h"

static PID pid;
static float Ki, Kd, DeltaK;
static uint8_t count = 0;
static int16_t Out;
	
void PID_Init(void)
{
	pid.Sv = 00;
	pid.Kp = 30;
	pid.T = 10;
	pid.Td = 2;
	pid.Ti = 50;
	pid.Ek_1 = 10;
	pid.SEk = 0;
	pid.out0 = 100;
	
	PWM_Init();
	Timer_Init();
}

void PID_SetSpeed(uint16_t Speed)
{
	pid.Sv = Speed;
}

void PID_Ctrl(void)
{
	pid.Ep = Encoder_Get1();
	pid.Pout = pid.Kp * pid.Ep;
	
	pid.Ek = pid.Sv - pid.Ep;
	pid.SEk += pid.Ek;
	Ki = pid.T / pid.Ti * pid.Kp;
	pid.Iout = Ki * pid.SEk;
	
	Kd = pid.Td * pid.Kp;
	DeltaK = pid.Ek - pid.Ek_1;
	pid.Dout = Kd * DeltaK / pid.T;
	
	Out = pid.Pout + pid.Iout + pid.Dout + pid.out0;;
	
	if (Out > 1000)
	{
		Out = Out % 1000;
	}
	else if (Out < 0)
	{
		Out = 0;
	}
	
	PWM_SetCompare1(Out);
	
	pid.Ek_1 = pid.Ek;
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		count ++;
		if (count == 10) 
		{
			PID_Ctrl();
			count = 0;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
