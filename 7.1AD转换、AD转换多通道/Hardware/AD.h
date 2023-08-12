#ifndef __ADC_H__
#define __ADC_H__

void AD_Init(void);
uint16_t AD_GetValue(uint8_t ADC_Channel);
void ADC_NVIC_Config(void);

#endif
