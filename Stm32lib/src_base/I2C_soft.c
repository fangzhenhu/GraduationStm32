#include <I2C_soft.h>
#include <delay.h>
#include <stdio.h>
void IIC_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PB 时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 PB
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7); //PB6,PB7 输出高
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据  ,很对
}	

//感觉不是很好
void IIC_Stop(void) 
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(5);
	IIC_SCL=1; 
	delay_us(1); //--------------------这个是我加的。
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(5);							   	
}


//等待应答信号到来
//返回值：1，接收应答失败
//        0 接收应答成功

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_SCL=0;// 假设 应答信号的时钟 也是 我的单片机控制。
	//IIC_SDA=1;//delay_us(1); //----------------这个 也是我改的。	我把先后顺序调了一下。 

	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 
	//printf("IIC_Waint_ok\r\n");
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void) // 这个是 当 单片机 为接收数据的时候。用来 发送应答 和不发送应答。
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1; // 还是那句话，当 scl 为高电平时 才开始接收数据。
	delay_us(2);
	IIC_SCL=0; 
}

//不产生ACK应答		    
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

//IIC发送一个字节
	//返回从机有无应答
	//1，有应答
	//0，无应答			  
void IIC_Send_Byte(u8 txd) //高位先传输。
{                        
	u8 t;   
	SDA_OUT(); 	    
	IIC_SCL=0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7) //这个表示 什么数据？ 1000 0000 ，txd 为要发送的数据。
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;		// 也就是 说 外设在 scl 为 高电平时 才读数据。
		delay_us(5); 
		IIC_SCL=0;	
		delay_us(2);
	}	 
} 	   

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_SDA=0;
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		IIC_SCL=0; // 难道是 单片机 通过控制 IIC_SCL 的电平来 决定器件的 数据发送。好像是，而且 当IIC 为第电平时 器件才发送数据
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
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK   
	return receive;
}



