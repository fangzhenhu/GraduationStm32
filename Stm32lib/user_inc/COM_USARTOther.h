#ifndef _COM_USART
#define  _COM_USART
#include "stm32f10x_usart.h"
#include <stdio.h>
//这个适合 用于 其他串口,但是也只是 限于 Usart2,和USART3；


uint32_t mRCC_APB1Periph;
void ComGPIO_Init(uint32_t RCC_APB1Periph)//在调用前配置 GPIO口，和开启串口时钟；
{	   	GPIO_InitTypeDef Gpiostruct;
  mRCC_APB1Periph=RCC_APB1Periph;

						//RCC_APB1PeriphClockCmd(RCC_APB1Periph_AFIO,ENABLE);不需要开启串口复用时钟
	
				 //恢复串口的默认值； 这句话因该在Main 函数中调用。  
	if (RCC_APB1Periph==RCC_APB1Periph_USART2)//如果是串口2
	{   USART_DeInit(USART2);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		Gpiostruct.GPIO_Pin=GPIO_Pin_2;
		Gpiostruct.GPIO_Mode=GPIO_Mode_AF_PP;//复用输出；
		Gpiostruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&Gpiostruct);

		Gpiostruct.GPIO_Pin=GPIO_Pin_3;
		Gpiostruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;// 浮空输入；

		GPIO_Init(GPIOA,&Gpiostruct);

	}
	else if (RCC_APB1Periph==RCC_APB1Periph_USART3)
	{
		USART_DeInit(USART3);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		Gpiostruct.GPIO_Pin=GPIO_Pin_10;
		Gpiostruct.GPIO_Mode=GPIO_Mode_AF_PP;//复用输出；
		Gpiostruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&Gpiostruct);

		Gpiostruct.GPIO_Pin=GPIO_Pin_11;
		Gpiostruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;// 浮空输入；

		GPIO_Init(GPIOC,&Gpiostruct);

	}
		 
	


	
	
}
void ComUsart_init(uint32_t RCC_APB1Periph)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity =  USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	if (RCC_APB1Periph==RCC_APB1Periph_USART2)//如果是串口2
	{  
		//初始化串口
		USART_Init(USART2, &USART_InitStructure);
			 
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//使能 接收数据中断；

		USART_Cmd(USART2,ENABLE);//使能串口；
	}

	else if (RCC_APB1Periph==RCC_APB1Periph_USART3)
	{
		USART_Init(USART3, &USART_InitStructure);

		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//使能 接收数据中断；

		USART_Cmd(USART3,ENABLE);//使能串口；
	}

	// 他们这里的工程师很喜欢把一个纯数字转换为某种结构体的指针；





}


 //配置中断向量表。
//既然有中断，就要有中断分组   但是这里面没有中断分组；
 
void ComNvic_Init(uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority)
{
	//这里 IRQ 表示Interrupt Request 即：中断请求。
	NVIC_InitTypeDef Nvicstruct;
	if (mRCC_APB1Periph==RCC_APB1Periph_USART2)
	{
		Nvicstruct.NVIC_IRQChannel=USART2_IRQn;
	}
	else if (mRCC_APB1Periph==RCC_APB1Periph_USART3)
	{
		Nvicstruct.NVIC_IRQChannel=USART3_IRQn;
	}
	Nvicstruct.NVIC_IRQChannelPreemptionPriority=mNVIC_IRQChannelPreemptionPriority;
	Nvicstruct.NVIC_IRQChannelSubPriority=mNVIC_IRQChannelSubPriority;
	Nvicstruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&Nvicstruct);//  配置中断优先级；


}

/*使用microLib的方法*/

int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (uint8_t) ch);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}	

	return ch;
}
#endif