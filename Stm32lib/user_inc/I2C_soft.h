#ifndef I2C_SOFT
#define I2C_SOFT
#include "sys.h"
 


//STM32 GPIO 相关寄存器  每个GPIO端口有两个32位配置寄存器(GPIOx_CRL，GPIOx_CRH)分别控制每个端口的高八位和低八位，
//	如果IO口是0-7号的话，则写CRL寄存器，如果IO口是8-15号的话，则写CRH寄存器，
//	两个32位数据寄存器(GPIOx_IDR，GPIOx_ODR)一个是只读作输入数据寄存器，一个是只写作输出寄存器，
//	一个32位置位/复位寄存器(GPIOx_BSRR)，一个16位复位寄存器(GPIOx_BRR)和一个32位锁定寄存器(GPIOx_LCKR)。
//	常用的IO端口寄存器只有四个：CRH，CRL，IDR，ODR。

//IO方向设置
//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;} 如果是 配置 GPIOB_pin_6 GPIO_PIn7 就不是用这个寄存器了
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
#define SDA_IN()   {GPIOB->CRL&= 0x0FFFFFFF;GPIOB->CRL|=0x80000000;} //GPIOB->CRL&= 0x0FFFFFFF 清除掉PB7原来的设置
#define SDA_OUT()  {GPIOB->CRL&= 0x0FFFFFFF;GPIOB->CRL|=0x30000000 ;}//GPIOB->CRL|=0x80000000  0x8表示上/下拉输入模式（做输入口用）
																	//GPIOB->CRL|=0x30000000  0x3表示推挽输出模式（作输出口用，50M速率）

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

#endif