#include "RCC_Configuration.h"
#include <stm32f10x.h>
#include <stm32f10x_flash.h>
/*! System Clocks Configuration */   
void RCC_Configuration(void)   
{  		ErrorStatus HSEStartUpStatus;
	//��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
	RCC_DeInit();  

	//���ⲿ���پ���
	RCC_HSEConfig(RCC_HSE_ON); 

	//�ȴ��ⲿ����ʱ��׼����
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

	//�ⲿ����ʱ���Ѿ�׼���
	if(HSEStartUpStatus == SUCCESS)  
	{

		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);//����� ���� flash ��ָ��Ĵ�ȡ��


		FLASH_SetLatency(FLASH_Latency_2);//�Ƚ�ֹFlash 2 �����ڵķ��ʣ�

		//����AHB(HCLK)ʱ��=SYSCLK  Ϊ72
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		//����APB2(PCLK2)��=AHBʱ�� Ϊ72 
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		//����APB1(PCLK1)��=AHB 1/2ʱ�� 36
		RCC_PCLK1Config(RCC_HCLK_Div2);  

		//����ADCʱ��=PCLK2 1/4
		RCC_ADCCLKConfig(RCC_PCLK2_Div4); 

		//����PLLʱ�� == �ⲿ���پ���ʱ��*9 72
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 

		//����ADCʱ��= PCLK2/4
		//  RCC_ADCCLKConfig(RCC_PCLK2_Div4);

		//ʹ��PLLʱ��
		RCC_PLLCmd(ENABLE);  

		//�ȴ�PLLʱ�Ӿ���
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)  
		{
		}

		//����ϵͳʱ�� = PLLʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

		//���PLLʱ���Ƿ���Ϊϵͳʱ��
		while(RCC_GetSYSCLKSource() != 0x08)  
		{
		}
	}

	//����GPIOʱ��


	//��������ʱ��



}

   