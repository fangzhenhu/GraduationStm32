/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM之STM32核心团队  JESSE 
* Version            :
* Date               : 
* Description        : STM32神舟III号开发板    广州
*******************************************************************************/
#include "led.h"

/*初始化4个LED灯*/
void LED_config(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;

	  /* 配置神舟III号LED灯使用的GPIO管脚模式*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE); /*使能LED灯使用的GPIO时钟*/

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  /*神州III号使用的LED灯相关的GPIO口初始化*/
}
