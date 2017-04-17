#include "stm32f10x.h"
#include "led.h"
#include  <RCC_Configuration.h>
#include <delay.h>
#include <usart.h>
#include <COM_USART1.h>
#include <usart.h>
#include <sys.h>
#include <ADC_init.h>
//#define LED0 PBout(5)// PB5
//#define LED1 PEout(5)// PE5	
#define LED1  PFout(6)
#define LED2  PFout(7)
extern __IO uint16_t ADCConvertedValueLocal	;
int main(void)
{
	uint16_t  Precent = 0, Voltage = 0;
	RCC_Configuration();//72Mhz
	delay_init(72000000);//��ʼ����ʱ������
	LED_config();	 //����4��LED�Ƴ�ʼ������
	//����1 ��ʼ�� -----�����ȥ��һ�� ����������һ��Ĵ��롣
	ComUsart_init();
	ComGPIO_Init();
	ComNvic_Init(2,2);
	//����2 ��ʼ��
	 uart2_init(115200);

	//adc ��ʼ��
	ADC_Configuration();
	printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  	while(1)
  	{	
		LED1=1;
		LED2=0;
		delay_ms(1000);
		LED1=0;
		LED2=1;
		delay_ms(1000);
		printf("nihao ,baby\n");
	//	ADCConvertedValueLocal = GetADCConvertedValue();
		Precent = (ADCConvertedValueLocal*100/0x1000);	//����ٷֱ�
		Voltage = Precent*33;						  // 3.3V�ĵ�ƽ�������Ч��ƽ
		printf("\r\n ��ǰADת�����Ϊ��0x%X, �ٷֱ�Ϊ��%d%%����ѹֵ��%d.%d%dV.\r\n", 
		ADCConvertedValueLocal, Precent, Voltage/1000, (Voltage%1000)/100, (Voltage%100)/10);

  	}
  
}


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
