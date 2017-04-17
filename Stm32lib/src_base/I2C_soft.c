#include <I2C_soft.h>
#include <delay.h>
#include <stdio.h>
void IIC_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PB ʱ��ʹ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ�� PB
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); //PB6,PB7 �����
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������  ,�ܶ�
}	

//�о����Ǻܺ�
void IIC_Stop(void) 
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(5);
	IIC_SCL=1; 
	delay_us(1); //--------------------������Ҽӵġ�
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(5);							   	
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0 ����Ӧ��ɹ�

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SCL=0;// ���� Ӧ���źŵ�ʱ�� Ҳ�� �ҵĵ�Ƭ�����ơ�
	//IIC_SDA=1;//delay_us(1); //----------------��� Ҳ���Ҹĵġ�	�Ұ��Ⱥ�˳�����һ�¡� 

	SDA_IN();      //SDA����Ϊ����  
	delay_us(2);
	IIC_SCL=1;delay_us(2);	 
	
	while(READ_SDA)
	{

		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			printf("IIC_Wait_ack error");
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 
	//printf("IIC_Waint_ok\r\n");
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void) // ����� �� ��Ƭ�� Ϊ�������ݵ�ʱ������ ����Ӧ�� �Ͳ�����Ӧ��
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1; // �����Ǿ仰���� scl Ϊ�ߵ�ƽʱ �ſ�ʼ�������ݡ�
	delay_us(2);
	IIC_SCL=0; 
}

//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//IIC����һ���ֽ�
	//���شӻ�����Ӧ��
	//1����Ӧ��
	//0����Ӧ��			  
void IIC_Send_Byte(u8 txd) //��λ�ȴ��䡣
{                        
	u8 t;   
	SDA_OUT(); 	    
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{              
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7) //�����ʾ ʲô���ݣ� 1000 0000 ��txd ΪҪ���͵����ݡ�
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;		// Ҳ���� ˵ ������ scl Ϊ �ߵ�ƽʱ �Ŷ����ݡ�
		delay_us(5); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	   

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA=0;
	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; // �ѵ��� ��Ƭ�� ͨ������ IIC_SCL �ĵ�ƽ�� ���������� ���ݷ��͡������ǣ����� ��IIC Ϊ�ڵ�ƽʱ �����ŷ�������
		delay_us(5);
		IIC_SCL=1;
		receive<<=1;

		if(READ_SDA)
		{
			receive++;   
		}
		delay_us(1); 
	}					 
	if (!ack)
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK   
	return receive;
}



