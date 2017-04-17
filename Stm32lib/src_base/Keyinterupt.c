#include "Keyinterupt.h"

/*
1:初始化想要设置中断的io 口。（注意时钟要先初始化，在这里没有初始化；)
2：
*/

void MInterruputInit(uint8_t GPIO_Portsource,uint8_t GPIO_Pinsource,uint32_t mEXIT_line)//这一步一定要复用功能时钟；
{       

 EXTI_InitTypeDef Exit_Initstruct;//第四步：用于配置中断， 服务；  
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//  第二步：复用功能时钟；
// 配置中断线 2；

GPIO_EXTILineConfig(GPIO_Portsource,GPIO_Pinsource);//第三步：配置 中断线 2； GPIOA 端口，pin_2; 都是从0开始；


Exit_Initstruct.EXTI_Line=mEXIT_line;// 我他妈感觉这个就是多余； 
Exit_Initstruct.EXTI_Mode=EXTI_Mode_Interrupt;//这个是中断类型；
Exit_Initstruct.EXTI_Trigger=EXTI_Trigger_Falling;//这个是中断触发方式；
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

void MNvic_Init(uint8_t mNVIC_IRQChannel,uint8_t mNVIC_IRQChannelSubPriority,uint8_t  mNVIC_IRQChannelPreemptionPriority)//别忘了要设置中断分组；
{
	NVIC_InitTypeDef m_NvicInit;
	m_NvicInit.NVIC_IRQChannel=mNVIC_IRQChannel;
	m_NvicInit.NVIC_IRQChannelSubPriority=mNVIC_IRQChannelSubPriority;
	m_NvicInit.NVIC_IRQChannelPreemptionPriority=mNVIC_IRQChannelPreemptionPriority;
	NVIC_Init(&m_NvicInit);

}

