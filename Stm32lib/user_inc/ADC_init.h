#ifndef ADC_INIT
#define  ADC_INIT

#include "stm32f10x.h"
/* ���ADCΪ12λģ��ת��������ı�����ֻ��ADCConvertedValue�ĵ�12λ��Ч */

void ADC_Configuration();
uint16_t GetADCConvertedValue(void);


#endif