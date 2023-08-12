#ifndef __SERIAL_H__
#define __SERIAL_H__

extern char RX_DataPackage[100];

void Serial_Init(void);
void Serial_SendByte(const uint8_t Data);
void Serial_SendString(const char *String);
uint8_t Serial_GetFlag(void);
void Serial_ClearFlag(void);

#endif
