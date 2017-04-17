#include <SPI_1.h>
#include <stm32f10x_spi.h>

void SPI_Port_init()
{  // 注意这里 不需要开启 AFIO 时钟，这只是端口本来的功能。
	//只有使用了AFIO的事件控制寄存器、AFIO的重映射功能以及外部中断(EXTI)控制寄存器才需要开启AFIO的时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOC, ENABLE );//PORTA，和SPI1 时钟使能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PB5/6/7 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 GPIOA

	//SPI1  NSS 这个是 用来操作 Flash 的cs;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4); // 问什么 要把GPIOC.4 置位高电平。现在不是太懂。
	/*  感觉 这个没有必要*/
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;*/

	/*	GPIO_Init(GPIOA, &GPIO_InitStructure);这个被改动过。
	GPIO_SetBits(GPIOA, GPIO_Pin_4);  */ 
}

void SPI_Config()
{
	SPI_InitTypeDef  Instruct_SPI;
	 Instruct_SPI.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	 // SPI_CPHA 用来设置时钟相位，也就是选择在串行同步时钟的第几个跳变沿（上升 或下降）数据被采样，可以为第一个或者第二个条边沿采集，这里我们选择第二个跳变沿，所  以选择 SPI_CPHA_2Edge
	 Instruct_SPI.SPI_CPHA=SPI_CPHA_2Edge;
	 // SPI_CPOL 用来设置时钟极性，我们设置串行同步时钟的空闲状态为高电平所以我 们选择 SPI_CPOL_High。
	 Instruct_SPI.SPI_CPOL=SPI_CPOL_High;
	 // PI_CRCPolynomial 是用来设置 CRC 校验多项式。比如：g(x)=x^3+x^2+2;=1101=13
	 Instruct_SPI.SPI_CRCPolynomial=7;// 我对 循环校检 有点小忘，反正这个就是 CRC 校验多项式。
	 Instruct_SPI.SPI_DataSize=SPI_DataSize_8b;
	 //SPI_Direction 是用来设置 SPI 的通信方式，可以选择为半双工，全双工，以及串行 发和串行收方式，这里我们选择全双工模式 SPI_Direction_2Lines_FullDuplex。
	 Instruct_SPI.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	 Instruct_SPI.SPI_FirstBit=SPI_FirstBit_MSB;// MSB 可以理解为 高位先传输；
	 Instruct_SPI.SPI_Mode=SPI_Mode_Master;
	// SPI_NSS 设置 NSS 信号由硬件（NSS 管脚）还是软件控制，这里我们通过软件控  制 NSS 关键，而不是硬件自动控制，所以选择 SPI_NSS_So
	 Instruct_SPI.SPI_NSS=SPI_NSS_Soft;


	SPI_Init(SPI1,&Instruct_SPI);

	//3 ）使能 SPI1 ,初始化完成之后接下来是要使能 SPI1 通信了，在使能 SPI1 之后，我们就可以开始 SPI 通 讯了。使能 SPI1 的方法是：
	SPI_Cmd(SPI1, ENABLE); //使能 SPI 外设

}

	//SPIx 读 写一个字节
	//TxData:要写入的字节
	//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	 
		// SPI_I2S_FLAG_TXE =2  ,Transmit buffer empty flag.
		// SPI_I2S_FLAG_RXNE=1: Receive buffer not empty flag.
	u8 retry=0;				 
	while((SPI1->SR&2)==0)//等待发送区空	 SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte 这句代码和 SPI_I2S_SendData(TxData); 等价。
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据	这句代码和 	SPI_I2S_ReceiveData(SPI1); 			    
}

/************************************************************************/
/* 
u8 SPI_ReadWriteByte(u8 TxData)
{ 
	u8 retry=0; 
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //等待发送区空
		{
		   retry++;
		   if(retry>200)return 0;
		} 
		SPI_I2S_SendData(SPI2, TxData); //通过外设 SPIx 发送一个数据
		   retry=0;
		 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //等待接收
		//完一个 byte 
		{
		   retry++;
		   if(retry>200)return 0;
		} 
		   return SPI_I2S_ReceiveData(SPI2); //返回通过 SPIx 最近接收的数据
}
*/
/************************************************************************/


