#ifndef _COM_USART
#define  _COM_USART
#include "stm32f10x_usart.h"

/*
 �����Ҫʹ�� printf();
 1������Ҫ�����main �ļ��� ������stdio.h�� ����׼�������ͷ�ļ�����

 2����main�ļ����ض���<fputc>���� ���£�

 // ��������

 int fputc(int ch, FILE *f)

 {

 USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��

 while (!(USART1->SR & USART_FLAG_TXE));

 return (ch);

 }

 // ��������

 int GetKey (void) {

 while (!(USART1->SR & USART_FLAG_RXNE));

 return ((int)(USART1->DR & 0x1FF));

 }
 ������ʹ��printfʱ�ͻ�����Զ����fputc�������������ַ���

 3���ڹ������Ե� ��Target" -> "Code Generation" ѡ���й�ѡ "Use MicroLIB"��

 MicroLIB ��ȱʡC�ı��ݿ⣬���������Ե����ϲ�����ϸ���ϡ�

 ����������ã��ڹ����п�������ʹ��printf�򴮿ڷ��������ˡ�
*/

void ComGPIO_Init(void);//�ڵ���ǰ����gpio ʱ�ӣ�
void ComUsart_init(void);

//������ û���жϷ��飻
void ComNvic_Init(uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority); 


//������ �����жϴ�������
//void USART1_IRQHandler(void) //�����ǽ��յ� �����ϵ�����ʱ��ز����жϣ�
//{   
//	if (USART_GetFlagStatus(USART1,USART_IT_RXNE)!=RESET)
//	{
//		char receive='a';
//
//		GPIO_Write(GPIOB,displaycode[1]);
//		Delay(10);// ������ Delay() �Ƿǳ������ǵ�ѡ��
//		// USART_ClearFlag(USART1,USART_FLAG_RXNE); ������������һ����
//		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
//
//
//		// ��־λ�ڳ����п�����Ϊ�ж�������֧�ֳ�������У��ж�������ת���жϺ���ִ�С�
//		//   ��������ʵ�ֵĹ�����һ���ģ����жϳ����п����������е���һ����������������������Ϊ�˸������ɡ�
//
//		//uint16_t receive;
//
//		receive=USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
//
//		USART_SendData(USART1,receive);
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//		//���ͽ����� ��ð� ��־λ���㣻
//		//��������жϱ�־λ
//		///////////////////////////��仰�������Ǵ�����ַ�������///////////////////////////////////////////////
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
