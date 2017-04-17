#include "COM_USART1.h"
#include <stdio.h>

	   //接收数据

 int GetKey (void) {

 while (!(USART1->SR & USART_FLAG_RXNE));

 return ((int)(USART1->DR & 0x1FF));

 }

 // 发送数据

 int fputc(int ch, FILE *f)

 {

 USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等

 while (!(USART1->SR & USART_FLAG_TXE));

 return (ch);

 }
void ComGPIO_Init() //在这里已经开启了 端口复用时钟；且开启的是串口一；
{
	GPIO_InitTypeDef Gpiostruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	Gpiostruct.GPIO_Pin=GPIO_Pin_9;
	Gpiostruct.GPIO_Mode=GPIO_Mode_AF_PP;//复用输出；
	Gpiostruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&Gpiostruct);

	Gpiostruct.GPIO_Pin=GPIO_Pin_10;
	Gpiostruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;// 浮空输入；

	GPIO_Init(GPIOA,&Gpiostruct);



}

void ComUsart_init()
{

	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity =  USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//初始化串口
	USART_Init(USART1, &USART_InitStructure);
	//启动串口   

	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能 接收数据中断；

	USART_Cmd(USART1,ENABLE);//使能串口；


	
}

void ComNvic_Init(uint8_t mNVIC_IRQChannelSubPriority,uint8_t mNVIC_IRQChannelPreemptionPriority)
{

	NVIC_InitTypeDef Nvicstruct;

	Nvicstruct.NVIC_IRQChannel=USART1_IRQn;
	Nvicstruct.NVIC_IRQChannelPreemptionPriority=mNVIC_IRQChannelPreemptionPriority;
	Nvicstruct.NVIC_IRQChannelSubPriority=mNVIC_IRQChannelSubPriority;
	Nvicstruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvicstruct);//  配置中断优先级；


}

