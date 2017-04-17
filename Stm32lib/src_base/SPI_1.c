#include <SPI_1.h>
#include <stm32f10x_spi.h>

void SPI_Port_init()
{  // ע������ ����Ҫ���� AFIO ʱ�ӣ���ֻ�Ƕ˿ڱ����Ĺ��ܡ�
	//ֻ��ʹ����AFIO���¼����ƼĴ�����AFIO����ӳ�书���Լ��ⲿ�ж�(EXTI)���ƼĴ�������Ҫ����AFIO��ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1|RCC_APB2Periph_GPIOC, ENABLE );//PORTA����SPI1 ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PB5/6/7 �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ�� GPIOA

	//SPI1  NSS ����� �������� Flash ��cs;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4); // ��ʲô Ҫ��GPIOC.4 ��λ�ߵ�ƽ�����ڲ���̫����
	/*  �о� ���û�б�Ҫ*/
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;*/

	/*	GPIO_Init(GPIOA, &GPIO_InitStructure);������Ķ�����
	GPIO_SetBits(GPIOA, GPIO_Pin_4);  */ 
}

void SPI_Config()
{
	SPI_InitTypeDef  Instruct_SPI;
	 Instruct_SPI.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	 // SPI_CPHA ��������ʱ����λ��Ҳ����ѡ���ڴ���ͬ��ʱ�ӵĵڼ��������أ����� ���½������ݱ�����������Ϊ��һ�����ߵڶ��������زɼ�����������ѡ��ڶ��������أ���  ��ѡ�� SPI_CPHA_2Edge
	 Instruct_SPI.SPI_CPHA=SPI_CPHA_2Edge;
	 // SPI_CPOL ��������ʱ�Ӽ��ԣ��������ô���ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ������ ��ѡ�� SPI_CPOL_High��
	 Instruct_SPI.SPI_CPOL=SPI_CPOL_High;
	 // PI_CRCPolynomial ���������� CRC У�����ʽ�����磺g(x)=x^3+x^2+2;=1101=13
	 Instruct_SPI.SPI_CRCPolynomial=7;// �Ҷ� ѭ��У�� �е�С��������������� CRC У�����ʽ��
	 Instruct_SPI.SPI_DataSize=SPI_DataSize_8b;
	 //SPI_Direction ���������� SPI ��ͨ�ŷ�ʽ������ѡ��Ϊ��˫����ȫ˫�����Լ����� ���ʹ����շ�ʽ����������ѡ��ȫ˫��ģʽ SPI_Direction_2Lines_FullDuplex��
	 Instruct_SPI.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	 Instruct_SPI.SPI_FirstBit=SPI_FirstBit_MSB;// MSB �������Ϊ ��λ�ȴ��䣻
	 Instruct_SPI.SPI_Mode=SPI_Mode_Master;
	// SPI_NSS ���� NSS �ź���Ӳ����NSS �ܽţ�����������ƣ���������ͨ�������  �� NSS �ؼ���������Ӳ���Զ����ƣ�����ѡ�� SPI_NSS_So
	 Instruct_SPI.SPI_NSS=SPI_NSS_Soft;


	SPI_Init(SPI1,&Instruct_SPI);

	//3 ��ʹ�� SPI1 ,��ʼ�����֮���������Ҫʹ�� SPI1 ͨ���ˣ���ʹ�� SPI1 ֮�����ǾͿ��Կ�ʼ SPI ͨ Ѷ�ˡ�ʹ�� SPI1 �ķ����ǣ�
	SPI_Cmd(SPI1, ENABLE); //ʹ�� SPI ����

}

	//SPIx �� дһ���ֽ�
	//TxData:Ҫд����ֽ�
	//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	 
		// SPI_I2S_FLAG_TXE =2  ,Transmit buffer empty flag.
		// SPI_I2S_FLAG_RXNE=1: Receive buffer not empty flag.
	u8 retry=0;				 
	while((SPI1->SR&2)==0)//�ȴ���������	 SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte ������� SPI_I2S_SendData(TxData); �ȼۡ�
	retry=0;
	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //�����յ�������	������� 	SPI_I2S_ReceiveData(SPI1); 			    
}

/************************************************************************/
/* 
u8 SPI_ReadWriteByte(u8 TxData)
{ 
	u8 retry=0; 
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //�ȴ���������
		{
		   retry++;
		   if(retry>200)return 0;
		} 
		SPI_I2S_SendData(SPI2, TxData); //ͨ������ SPIx ����һ������
		   retry=0;
		 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //�ȴ�����
		//��һ�� byte 
		{
		   retry++;
		   if(retry>200)return 0;
		} 
		   return SPI_I2S_ReceiveData(SPI2); //����ͨ�� SPIx ������յ�����
}
*/
/************************************************************************/


