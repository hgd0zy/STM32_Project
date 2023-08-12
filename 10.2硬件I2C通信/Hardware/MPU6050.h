#ifndef __MPU6050_H__
#define __MPU6050_H__

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_WReg(uint8_t Reg, uint8_t Command);
uint8_t MPU6050_RReg(uint8_t Reg);
void MPU6050_WriteRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length);
void MPU6050_ReadRegArray(uint8_t *Array, uint8_t Reg, uint8_t Length);
void MPU6050_GetACCEL(int16_t *Array);
void MPU6050_GetGYRO(int16_t *Array);
uint8_t MPU6050_Test(void);

#endif
