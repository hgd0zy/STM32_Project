#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "PWM.h"
#include "Timer.h"
#include "Serial.h"

#define ABS(x) ((x) = (x)<0?-(x):(x))

static PID pid1, pid2;
static float Ki, Kd, DeltaK1, DeltaK2;
static float Out1, Out2;
int16_t x1, x2;
	
void PID_Init(void)
{
	pid1.Sv = 00;
	pid2.Sv = 00;
	pid1.out0 = 10;
	pid2.out0 = 10;
	pid1.SEk = 0;
	pid2.SEk = 0;
	pid1.Ek_1 = 5;
	pid2.Ek_1 = 5;
	
	pid1.Kp = 150;
	pid1.Ti = 400;
	pid1.T = 50;
	pid1.Td = 1.1;
	
	pid2.Kp = 150;
	pid2.Ti = 400;
	pid2.T = 50;
	pid2.Td = 1.1;
	
	Ki = pid1.T / pid1.Ti * pid1.Kp;
	Kd = pid1.Td * pid1.Kp;
	
	Timer_Init();
}

void PID_SetSpeed(uint8_t Speed)
{
	pid1.Sv = Speed;
	pid2.Sv = Speed;
}

void PID_Ctrl(int16_t l_speed, int16_t r_speed)
{
	pid1.Ep = ABS(l_speed);
	pid2.Ep = ABS(r_speed);
	
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
	
	//Serial_printf("%d, %d, %d\n", pid1.Ep, pid2.Ep, pid1.Sv);//利用VOFA输出波形
}

