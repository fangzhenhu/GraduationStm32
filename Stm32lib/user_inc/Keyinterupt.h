//************************************************************************/
//* һ�������жϡ�                                                                     */
//************************************************************************/
#ifndef _mEXit_init
#define  _mEXit_init
#include "stm32f10x_exti.h" 
//�������� ��Ҫ���ݲ�����  ע�⣺ ��û�� ʹ��GPIO ʱ��
void mGPIO_init(GPIO_TypeDef* GPIOx,uint16_t mGPIO_Pin);

// ������û�н����жϷ��飻 mNVIC_IRQChannel ����Ϊ  EXTI0_IRQn     
void MNvic_Init(uint8_t mNVIC_IRQChannel,uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority);

/*
 @��Ҫ����д�жϴ�������

*/
void MInterruputInit(uint8_t GPIO_Portsource,uint8_t GPIO_Pinsource,uint32_t mEXIT_line);

#endif


