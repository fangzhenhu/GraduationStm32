#ifndef I2C_SOFT
#define I2C_SOFT
#include "sys.h"
 


//STM32 GPIO ��ؼĴ���  ÿ��GPIO�˿�������32λ���üĴ���(GPIOx_CRL��GPIOx_CRH)�ֱ����ÿ���˿ڵĸ߰�λ�͵Ͱ�λ��
//	���IO����0-7�ŵĻ�����дCRL�Ĵ��������IO����8-15�ŵĻ�����дCRH�Ĵ�����
//	����32λ���ݼĴ���(GPIOx_IDR��GPIOx_ODR)һ����ֻ�����������ݼĴ�����һ����ֻд������Ĵ�����
//	һ��32λ��λ/��λ�Ĵ���(GPIOx_BSRR)��һ��16λ��λ�Ĵ���(GPIOx_BRR)��һ��32λ�����Ĵ���(GPIOx_LCKR)��
//	���õ�IO�˿ڼĴ���ֻ���ĸ���CRH��CRL��IDR��ODR��

//IO��������
//#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;} ����� ���� GPIOB_pin_6 GPIO_PIn7 �Ͳ���������Ĵ�����
//#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
#define SDA_IN()   {GPIOB->CRL&= 0x0FFFFFFF;GPIOB->CRL|=0x80000000;} //GPIOB->CRL&= 0x0FFFFFFF �����PB7ԭ��������
#define SDA_OUT()  {GPIOB->CRL&= 0x0FFFFFFF;GPIOB->CRL|=0x30000000 ;}//GPIOB->CRL|=0x80000000  0x8��ʾ��/��������ģʽ����������ã�
																	//GPIOB->CRL|=0x30000000  0x3��ʾ�������ģʽ����������ã�50M���ʣ�

//IO��������	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);

#endif