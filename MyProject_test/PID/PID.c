#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "PWM.h"
#include "Encoder.h"
#include "Timer.h"
#include "Serial.h"

static PID pid1, pid2;
static float Ki, Kd, DeltaK1, DeltaK2;
static uint8_t count = 0;
static float Out1, Out2;
	
void PID_Init(void)
{
	pid1.Sv = 00;
	pid1.Kp = 400;
	pid1.Ti = 800;
	pid1.SEk = 0;
	pid1.T = 100;
	pid1.Ek_1 = 5;
	pid1.Td = 0.5;
	pid1.out0 = 10;
	
	pid2.Sv = 00;
	pid2.Kp = 400;
	pid2.Ti = 800;
	pid2.SEk = 0;
	pid2.T = 100;
	pid2.Ek_1 = 5;
	pid2.Td = 0.5;
	pid2.out0 = 10;
	
	Ki = pid1.T / pid1.Ti * pid1.Kp;
	Kd = pid1.Td * pid1.Kp;
	
	PWM_Init();
	Timer_Init();
}

void PID_SetSpeed(uint16_t Speed)
{
	pid1.Sv = Speed;
	pid2.Sv = Speed;
}

void PID_Ctrl(void)
{
	pid1.Ep = Encoder_Get1();
	pid2.Ep = Encoder_Get2();
	
	//比例控制
	pid1.Ek = pid1.Sv - pid1.Ep;
	pid1.Pout = pid1.Kp * pid1.Ek;
	pid2.Ek = pid2.Sv - pid2.Ep;
	pid2.Pout = pid2.Kp * pid2.Ek;
	
	//积分控制
	pid1.SEk += pid1.Ek;
	if (pid1.SEk > 450) {pid1.SEk = 450;}
	else if (pid1.SEk < -450) {pid1.SEk = -450;}
	pid1.Iout = Ki * pid1.SEk;
	pid2.SEk += pid2.Ek;
	if (pid2.SEk > 450) {pid2.SEk = 450;}
	else if (pid2.SEk < -450) {pid2.SEk = -450;}
	pid2.Iout = Ki * pid2.SEk;
	
	//微分控制
	DeltaK1 = pid1.Ek - pid1.Ek_1;
	pid1.Dout = Kd * DeltaK1 / pid1.T;
	DeltaK2 = pid2.Ek - pid2.Ek_1;
	pid2.Dout = Kd * DeltaK2 / pid2.T;
	
	Out1 = pid1.Pout + pid1.Iout + pid1.Dout + pid1.out0;
	Out2 = pid2.Pout + pid2.Iout + pid2.Dout + pid2.out0;
	
	if (Out1 > 7200){Out1 = 7200;}
	else if (Out1 < 0){Out1 = 0;}
	if (Out2 > 7200){Out2 = 7200;}
	else if (Out2 < 0){Out2 = 0;}
	
	PWM_SetCompare1(Out1);
	PWM_SetCompare2(Out2);
	
	pid1.Ek_1 = pid1.Ek;
	pid2.Ek_1 = pid2.Ek;
	
	Serial_printf("%d, %d, %d\n", pid1.Ep, pid2.Ep, pid1.Sv);//利用VOFA输出波形
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		count ++;
		if (count == 2) 
		{
			PID_Ctrl();
			count = 0;
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
