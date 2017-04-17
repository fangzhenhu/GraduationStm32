#include <ADC_init.h>
#include "stm32f10x_adc.h"
#include <stm32f10x_dma.h>

	  /* ADC1 DR register base address */
#define DR_ADDRESS                  ((uint32_t)0x4001244C)
__IO uint16_t ADCConvertedValue;     // 
void ADC_GPIO_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOC clock */
	/* ʹ��GPIOCʱ��      */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  //

	/* Configure PA.01 (ADC Channel12) as analog input -------------------------*/
	//PC3 ��Ϊģ��ͨ��13��������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       //�ܽ�3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //����ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);          //GPIO��

}
__IO uint16_t ADCConvertedValueLocal;//����һ��
void ADC_Configuration()
{

	ADC_InitTypeDef ADC_InitStructure;        //ADC��ʼ���ṹ������
	DMA_InitTypeDef DMA_InitStructure;        //DMA��ʼ���ṹ������     

	ADC_GPIO_Configuration(); //���� GPIO ��

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //ʹ��DMAʱ��

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);		  // //��DMA1 ͨ��1���мĴ�������Ϊȱʡֵ  
	DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  //DMA��Ӧ���������ַ �������DR_ADDRESS ��ʾ STM32оƬADCת�����DR�Ĵ�������ַ 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValueLocal;//(uint32_t)&ADCConvertedValue;   //�ڴ�洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
	DMA_InitStructure.DMA_BufferSize = 1;		   //DMA�����С��1��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//����һ�����ݺ��豸��ַ��ֹ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//�رգ�����һ�����ݺ�Ŀ���ڴ��ַ���ƣ��� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�����������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA�������ݳߴ磬HalfWord����Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //ת��ģʽ��ѭ������ģʽ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA���ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2Mģʽ����
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADCʱ��

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//������ת��ģʽ   �����ģʽ�£�˫ADC����ͬ����ÿ��ADC�ӿڶ��������������������ҪADCͬ������ֻ������һ��ADC��ʱ�򣬾�Ӧ����ɶ���ģʽ�ˡ�
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //����ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //��������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC�ⲿ���أ��ر�״̬
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //���뷽ʽ,ADCΪ12λ�У��Ҷ��뷽ʽ
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 //����ͨ������1��
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel10 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);
	//ADCͨ���飬 ��13��ͨ�� ����˳��1��ת��ʱ�� 
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);	  //ADC���ʹ��
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);  //����ADC1

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);	  //����У׼
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ�����У׼���
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);		//��ʼУ׼
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));	   //�ȴ�У׼���
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM����
}

/**-------------------------------------------------------
  * @������ GetADCConvertedValue
  * @����   ��ȡADCģ��ת���Ľ��
  * @����   ��
  * @����ֵ ADCת������ı�����ֻ�еĵ�12λ��Ч
***------------------------------------------------------*/
uint16_t GetADCConvertedValue(void)
{
    return ADCConvertedValue;
}
