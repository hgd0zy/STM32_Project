#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * @brief 舵机初始化函数
  * @param 无
  * @retval 无
  */
void Servo_Init(void)
{
	PWM_Init();	
}

/**
  * @brief 设置舵机旋转角度
  * @param 角度
  * @retval 无
  */
void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

