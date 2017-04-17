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
	/* 使能GPIOC时钟      */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  //

	/* Configure PA.01 (ADC Channel12) as analog input -------------------------*/
	//PC3 作为模拟通道13输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       //管脚3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //输入模式
	GPIO_Init(GPIOC, &GPIO_InitStructure);          //GPIO组

}
__IO uint16_t ADCConvertedValueLocal;//测试一把
void ADC_Configuration()
{

	ADC_InitTypeDef ADC_InitStructure;        //ADC初始化结构体声明
	DMA_InitTypeDef DMA_InitStructure;        //DMA初始化结构体声明     

	ADC_GPIO_Configuration(); //配置 GPIO 口

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		 //使能DMA时钟

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);		  // //将DMA1 通道1所有寄存器重设为缺省值  
	DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  //DMA对应的外设基地址 ，而这个DR_ADDRESS 表示 STM32芯片ADC转换结果DR寄存器基地址 
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValueLocal;//(uint32_t)&ADCConvertedValue;   //内存存储基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA的转换模式为SRC模式，由外设搬移到内存
	DMA_InitStructure.DMA_BufferSize = 1;		   //DMA缓存大小，1个
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//接收一次数据后，设备地址禁止后移
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;	//关闭（接收一次数据后，目标内存地址后移）， 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA搬移数据尺寸，HalfWord就是为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //转换模式，循环缓存模式。
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M模式禁用
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC时钟

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//独立的转换模式   ，这个模式下，双ADC不能同步，每个ADC接口独立工作。所以如果不需要ADC同步或者只是用了一个ADC的时候，就应该设成独立模式了。
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //开启扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //开启连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC外部开关，关闭状态
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //对齐方式,ADC为12位中，右对齐方式
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 //开启通道数，1个
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel10 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);
	//ADC通道组， 第13个通道 采样顺序1，转换时间 
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);	  //ADC命令，使能
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);  //开启ADC1

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);	  //重新校准
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);		//开始校准
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));	   //等待校准完成
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//连续转换开始，ADC通过DMA方式不断的更新RAM区。
}

/**-------------------------------------------------------
  * @函数名 GetADCConvertedValue
  * @功能   获取ADC模数转换的结果
  * @参数   无
  * @返回值 ADC转换结果的变量，只有的低12位有效
***------------------------------------------------------*/
uint16_t GetADCConvertedValue(void)
{
    return ADCConvertedValue;
}
