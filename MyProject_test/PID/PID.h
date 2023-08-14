#ifndef __PID_H__
#define __PID_H__

typedef struct PID{

	uint16_t Sv; //设定的目标值
	uint16_t Ep; //当前的测量值
	
	int16_t Kp; //当前参数
	float Ti; //积分常量
	float Td; //微分常量
	int16_t T;  //PID控制周期
	
	int16_t Ek; //当前误差
	int16_t Ek_1; //上一次的误差
	int32_t SEk; //历史误差
	
	float Pout; //比例输出
	float Iout; //积分输出
	float Dout; //微分输出
	
	int16_t out0;
}PID;

void PID_Init(void);
void PID_SetSpeed(uint16_t Speed);

#endif
