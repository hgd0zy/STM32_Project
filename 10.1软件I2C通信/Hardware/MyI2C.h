#ifndef __I2C_H__
#define __I2C_H__

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_STOP(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendACK(uint8_t ACKbit);
uint8_t MyI2C_ReceiveACK(void);


#endif
