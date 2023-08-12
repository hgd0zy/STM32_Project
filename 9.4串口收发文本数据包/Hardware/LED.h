#ifndef __LED_H__
#define __LED_H__

uint8_t LED[3] = {255, 0, 1};

void LED_Init(void); // Init LED A0,A1....
void LED_ON(uint8_t num); // LED0,LED1....
void LED_OFF(uint8_t num); // LED0,LED1....
void LED_Turn(uint8_t num); //取反

#endif
