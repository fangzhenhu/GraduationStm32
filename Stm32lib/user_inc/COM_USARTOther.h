#ifndef _COM_USART
#define  _COM_USART
#include "stm32f10x_usart.h"
#include <stdio.h>
//����ʺ� ���� ��������,����Ҳֻ�� ���� Usart2,��USART3��


uint32_t mRCC_APB1Periph;
void ComGPIO_Init(uint32_t RCC_APB1Periph)//�ڵ���ǰ���� GPIO�ڣ��Ϳ�������ʱ�ӣ�
{	   	GPIO_InitTypeDef Gpiostruct;
  mRCC_APB1Periph=RCC_APB1Periph;

						//RCC_APB1PeriphClockCmd(RCC_APB1Periph_AFIO,ENABLE);����Ҫ�������ڸ���ʱ��
	
				 //�ָ����ڵ�Ĭ��ֵ�� ��仰�����Main �����е��á�  
	if (RCC_APB1Periph==RCC_APB1Periph_USART2)//����Ǵ���2
	{   USART_DeInit(USART2);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
		Gpiostruct.GPIO_Pin=GPIO_Pin_2;
		Gpiostruct.GPIO_Mode=GPIO_Mode_AF_PP;//���������
		Gpiostruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&Gpiostruct);

		Gpiostruct.GPIO_Pin=GPIO_Pin_3;
		Gpiostruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;// �������룻

		GPIO_Init(GPIOA,&Gpiostruct);

	}
	else if (RCC_APB1Periph==RCC_APB1Periph_USART3)
	{
		USART_DeInit(USART3);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		Gpiostruct.GPIO_Pin=GPIO_Pin_10;
		Gpiostruct.GPIO_Mode=GPIO_Mode_AF_PP;//���������
		Gpiostruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOC,&Gpiostruct);

		Gpiostruct.GPIO_Pin=GPIO_Pin_11;
		Gpiostruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;// �������룻

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
	if (RCC_APB1Periph==RCC_APB1Periph_USART2)//����Ǵ���2
	{  
		//��ʼ������
		USART_Init(USART2, &USART_InitStructure);
			 
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//ʹ�� ���������жϣ�

		USART_Cmd(USART2,ENABLE);//ʹ�ܴ��ڣ�
	}

	else if (RCC_APB1Periph==RCC_APB1Periph_USART3)
	{
		USART_Init(USART3, &USART_InitStructure);

		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//ʹ�� ���������жϣ�

		USART_Cmd(USART3,ENABLE);//ʹ�ܴ��ڣ�
	}

	// ��������Ĺ���ʦ��ϲ����һ��������ת��Ϊĳ�ֽṹ���ָ�룻





}


 //�����ж�������
//��Ȼ���жϣ���Ҫ���жϷ���   ����������û���жϷ��飻
 
void ComNvic_Init(uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority)
{
	//���� IRQ ��ʾInterrupt Request �����ж�����
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
	NVIC_Init(&Nvicstruct);//  �����ж����ȼ���


}

/*ʹ��microLib�ķ���*/

int fputc(int ch, FILE *f)
{
	USART_SendData(USART2, (uint8_t) ch);

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}	

	return ch;
}
#endif