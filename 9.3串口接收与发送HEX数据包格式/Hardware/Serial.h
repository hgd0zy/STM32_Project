#ifndef __SERIAL_H__
#define __SERIAL_H__

extern uint8_t TX_DataPackage[5];
extern uint8_t RX_DataPackage[5];

void Serial_Init(void);
void Serial_SendByte(const uint8_t Data);
void Serial_SendArray(const uint8_t *Array, uint16_t len);
void Serial_SendString(const char *String);
void Serial_SendPackage(void);
uint8_t Serial_GetFlag(void);

#endif
