#ifndef _COM_USART
#define  _COM_USART
#include "stm32f10x_usart.h"

/*
 如果想要使用 printf();
 1、首先要在你的main 文件中 包含“stdio.h” （标准输入输出头文件）。

 2、在main文件中重定义<fputc>函数 如下：

 // 发送数据

 int fputc(int ch, FILE *f)

 {

 USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等

 while (!(USART1->SR & USART_FLAG_TXE));

 return (ch);

 }

 // 接收数据

 int GetKey (void) {

 while (!(USART1->SR & USART_FLAG_RXNE));

 return ((int)(USART1->DR & 0x1FF));

 }
 这样在使用printf时就会调用自定义的fputc函数，来发送字符。

 3、在工程属性的 “Target" -> "Code Generation" 选项中勾选 "Use MicroLIB"”

 MicroLIB 是缺省C的备份库，关于它可以到网上查找详细资料。

 至此完成配置，在工程中可以随意使用printf向串口发送数据了。
*/

void ComGPIO_Init(void);//在调用前开启gpio 时钟；
void ComUsart_init(void);

//这里面 没有中断分组；
void ComNvic_Init(uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority); 


//这里是 串口中断处理函数；
//void USART1_IRQHandler(void) //这里是接收到 电脑上的数据时候回产生中断；
//{   
//	if (USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET)
//	{
//		char receive='a';
//
//		GPIO_Write(GPIOB,displaycode[1]);
//		Delay(10);// 在这里 Delay() 是非常不明智的选择；
//		// USART_ClearFlag(USART1,USART_FLAG_RXNE); 这两个功能是一样的
//		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
//
//
//		// 标志位在程序中可以作为判定条件，支持程序的运行，中断则是跳转到中断函数执行。
//		//   两个函数实现的功能是一样的，在中断程序中可以用两个中的任一个。我想区分两个函数是为了更清晰吧。
//
//		//uint16_t receive;
//
//		receive=USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
//
//		USART_SendData(USART1,receive);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//		//发送结束后 最好把 标志位置零；
//		//清除接收中断标志位
//		///////////////////////////这句话的作用是处理多字符串发送///////////////////////////////////////////////
//		GPIO_Write(GPIOB,displaycode[2]);						
//		if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
//		{
//			//  USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
//			USART_ClearFlag(USART1,USART_FLAG_RXNE);
//		}
//	}
//
//
//}
#endif
