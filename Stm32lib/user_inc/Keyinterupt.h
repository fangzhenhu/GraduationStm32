//************************************************************************/
//* 一：按键中断。                                                                     */
//************************************************************************/
#ifndef _mEXit_init
#define  _mEXit_init
#include "stm32f10x_exti.h" 
//在这里面 ，要传递参数，  注意： 还没有 使能GPIO 时钟
void mGPIO_init(GPIO_TypeDef* GPIOx,uint16_t mGPIO_Pin);

// 在这里没有进行中断分组； mNVIC_IRQChannel 可以为  EXTI0_IRQn     
void MNvic_Init(uint8_t mNVIC_IRQChannel,uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority);

/*
 @不要忘了写中断处理函数；

*/
void MInterruputInit(uint8_t GPIO_Portsource,uint8_t GPIO_Pinsource,uint32_t mEXIT_line);

#endif


