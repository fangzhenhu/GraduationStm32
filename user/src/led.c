/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM֮STM32�����Ŷ�  JESSE 
* Version            :
* Date               : 
* Description        : STM32����III�ſ�����    ����
*******************************************************************************/
#include "led.h"

/*��ʼ��4��LED��*/
void LED_config(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;

	  /* ��������III��LED��ʹ�õ�GPIO�ܽ�ģʽ*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE); /*ʹ��LED��ʹ�õ�GPIOʱ��*/

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  /*����III��ʹ�õ�LED����ص�GPIO�ڳ�ʼ��*/
}
