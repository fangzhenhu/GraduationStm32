#ifndef __SPI_1
#define __SPI_1
#include "stm32f10x.h"
//1 ）配置相关引脚的复用功能，使能 SPI1 
void SPI_Port_init();
//2 ）初始化 SPI1, 设置 SPI1  工作模式
void SPI_Config();

u8 SPIx_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
/************************************************************************/
/* 4 ）SPI  传输数据
通信接口当然需要有发送数据和接受数据的函数，固件库提供的发送数据函数原型为：
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data)；
这个函数很好理解，往 SPIx 数据寄存器写入数据 Data，从而实现发送。
固件库提供的接受数据函数原型为：
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx) ；
这个函数也不难理解，从 SPIx 数据寄存器读出接受到的数据。

5 ）查看 SPI  传输状态
在 SPI 传输过程中，我们经常要判断数据是否传输完成，发送区是否为空等等状态，这是
通过函数 SPI_I2S_GetFlagStatus 实现的，这个函数很简单就不详细讲解，判断发送是否完成的
方法是：
SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)；                                                                     */
/********************************************************************** */

#endif
