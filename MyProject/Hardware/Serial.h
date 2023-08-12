#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stdio.h"

void Serial_Init(void);
void Serial_SendByte(const uint8_t Data);
void Serial_SendArray(const uint8_t *Array, uint16_t len);
void Serial_SendString(const char *String);
void Serial_SendNum(const uint32_t number, uint8_t length);
void Serial_printf(char *format, ...);
uint8_t Serial_GetFlag(void);
uint8_t Serial_GetRX(void);

#endif
