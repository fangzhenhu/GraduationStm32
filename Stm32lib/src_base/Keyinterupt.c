#include "Keyinterupt.h"

/*
1:��ʼ����Ҫ�����жϵ�io �ڡ���ע��ʱ��Ҫ�ȳ�ʼ����������û�г�ʼ����)
2��
*/

void MInterruputInit(uint8_t GPIO_Portsource,uint8_t GPIO_Pinsource,uint32_t mEXIT_line)//��һ��һ��Ҫ���ù���ʱ�ӣ�
{       

 EXTI_InitTypeDef Exit_Initstruct;//���Ĳ������������жϣ� ����  
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//  �ڶ��������ù���ʱ�ӣ�
// �����ж��� 2��

GPIO_EXTILineConfig(GPIO_Portsource,GPIO_Pinsource);//������������ �ж��� 2�� GPIOA �˿ڣ�pin_2; ���Ǵ�0��ʼ��


Exit_Initstruct.EXTI_Line=mEXIT_line;// ������о�������Ƕ��ࣻ 
Exit_Initstruct.EXTI_Mode=EXTI_Mode_Interrupt;//������ж����ͣ�
Exit_Initstruct.EXTI_Trigger=EXTI_Trigger_Falling;//������жϴ�����ʽ��
Exit_Initstruct.EXTI_LineCmd=ENABLE;
EXTI_Init(&Exit_Initstruct); //3
}
void mGPIO_init(GPIO_TypeDef* GPIOx,uint16_t mGPIO_Pin)//
{
	GPIO_InitTypeDef mInitgpio;
	mInitgpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	mInitgpio.GPIO_Pin=mGPIO_Pin;


	GPIO_Init(GPIOx,&mInitgpio);

}

void MNvic_Init(uint8_t mNVIC_IRQChannel,uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority)//������Ҫ�����жϷ��飻
{
	NVIC_InitTypeDef m_NvicInit;
	m_NvicInit.NVIC_IRQChannel=mNVIC_IRQChannel;
	m_NvicInit.NVIC_IRQChannelSubPriority=mNVIC_IRQChannelSubPriority;
	m_NvicInit.NVIC_IRQChannelPreemptionPriority=mNVIC_IRQChannelPreemptionPriority;
	NVIC_Init(&m_NvicInit);

}

