#ifndef ADC_INIT
#define  ADC_INIT

#include "stm32f10x.h"
/* 存放ADC为12位模数转换器结果的变量，只有ADCConvertedValue的低12位有效 */

void ADC_Configuration();
uint16_t GetADCConvertedValue(void);


#endif