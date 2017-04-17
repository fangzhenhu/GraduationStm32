#ifndef __LED_H
#define __LED_H	
#include "stm32f10x.h" 

/*����III��LED����ض���*/
#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF    /*LEDʹ�õ�GPIOʱ��*/
#define LEDn                            4                       /*����III��LED����*/
#define GPIO_LED                        GPIOF                   /*����III��LED��ʹ�õ�GPIO��*/

#define DS1_PIN                         GPIO_Pin_6              /*DS1ʹ�õ�GPIO�ܽ�*/
#define DS2_PIN                         GPIO_Pin_7				/*DS2ʹ�õ�GPIO�ܽ�*/
#define DS3_PIN                         GPIO_Pin_8  			/*DS3ʹ�õ�GPIO�ܽ�*/
#define DS4_PIN                         GPIO_Pin_9				/*DS4ʹ�õ�GPIO�ܽ�*/

void LED_config(void);
		 				    
#endif


