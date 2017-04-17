#include <I2C_EE.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_gpio.h>
#include <stdio.h>
#define I2C_Speed  400000 
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           8 // �������PageSize ������� ��Ϊ ���ݰ�

#define  EEP_Firstpage      0x00 //Ϊʲô ���ﻹ�� ҳ�ĸ��
#define  EEP_Randompage	    0x06
void GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	/* Configure I2C1 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void I2C_Configuration()
{
	I2C_InitTypeDef  I2C_InitStructure; 
 
	/* I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; //������ ���� I2C	��ģʽ ��һ��������ģʽ ��I2C_Mode_I2C  I2C_Mode_SMBusDevice  I2C_Mode_SMBusHost ����
	
	
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;// ���� �������� I2C ��ռ�ձ� ���� ռ�ձ�Ϊ Tlow/THigh=2
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7; //��� I2C_OwnAddress1 Ӧ�ñ�ʾ �Լ��ĵ�ַ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;// ����� ʹ�� Ӧ��
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // ��ʾ Ӧ���ַ���� 7 λ ����10 λ
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;//�ò�����������ʱ��Ƶ�ʣ����ֵ���ܸ���400kHz(1Hz---400KHz)

	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	printf("\r\n I2C_Configuration----\r\n");

}
 void I2C_EE_Init()
{
	/* GPIO configuration */
	GPIO_Configuration();

	/* ������ I2C ������  */
	I2C_Configuration();

	/* depending on the EEPROM Address selected in the i2c_ee.h file */
#ifdef EEPROM_Block0_ADDRESS
	/* Select the EEPROM Block0 to write on */
	EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif
#ifdef EEPROM_Block1_ADDRESS
	/* Select the EEPROM Block1 to write on */
	EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif
#ifdef EEPROM_Block2_ADDRESS
	/* Select the EEPROM Block2 to write on */
	EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif
#ifdef EEPROM_Block3_ADDRESS
	/* Select the EEPROM Block3 to write on */
	EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}

 void I2C_EE_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
 {
	 while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008 ���� ����Ҫ�������

	 /* ������������ */
	 I2C_GenerateSTART(I2C1, ENABLE);	//�⺯��

	 /* Test on EV5 and clear it */
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 

	 /* Send EEPROM address for write */
	 I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

	 /* Test on EV6 and clear it */
	 while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	 /* Send the EEPROM's internal address to write to */    
	 I2C_SendData(I2C1, WriteAddr);  

	 /* Test on EV8 and clear it */
	 while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	 /* While there is data to be written */
	 while(NumByteToWrite--)  
	 {
		 /* Send the current byte */
		 I2C_SendData(I2C1, *pBuffer); 

		 /* Point to the next byte to be written */
		 pBuffer++; 

		 /* Test on EV8 and clear it */
		 while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	 }

	 /* Send STOP condition */
	 I2C_GenerateSTOP(I2C1, ENABLE);
 }

 void I2C_EE_WaitEepromStandbyState(void)      
 {
	 vu16 SR1_Tmp = 0;

	 do
	 {
		 /* Send START condition */
		 I2C_GenerateSTART(I2C1, ENABLE);
		 /* Read I2C1 SR1 register */
		 SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
		 /* Send EEPROM address for write */
		 I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	 }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));

	 /* Clear AF flag */
	 I2C_ClearFlag(I2C1, I2C_FLAG_AF);
	 /* STOP condition */    
	 I2C_GenerateSTOP(I2C1, ENABLE); // Added by Najoua 27/08/2008
 }

/************************************************************************/
/* ��buffer �е����ݵ�����д��EERPOM�� 
	PBuffer �� buffer ָ��
	WriteAddr ��
	NumByteToWrite: һ��������
*/
/************************************************************************/
 void I2C_EE_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
 {
	 u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

	 Addr = WriteAddr % I2C_PageSize;  //0x00%8=0
	 count = I2C_PageSize - Addr;		//8-0=8
	 NumOfPage =  NumByteToWrite / I2C_PageSize;	//256/8	=32 Ҳ���� 32 �����ݰ�
	 NumOfSingle = NumByteToWrite % I2C_PageSize;	//256%8	=0  ���Single ��ʾ ���� �ղ���һ�����ݰ��������ʣ��� ����λ��bit)

	 /* If WriteAddr is I2C_PageSize aligned  */
	 if(Addr == 0) 
	 {
		 /* If NumByteToWrite < I2C_PageSize */
		 if(NumOfPage == 0) 
		 {
			 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			 I2C_EE_WaitEepromStandbyState();
		 }
		 /* If NumByteToWrite > I2C_PageSize */
		 else  
		 {
			 while(NumOfPage--)  //һ��д8������
			 {
				 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
				 I2C_EE_WaitEepromStandbyState();
				 WriteAddr +=  I2C_PageSize;
				 pBuffer += I2C_PageSize;
			 }

			 if(NumOfSingle!=0)	//��8�������
			 {
				 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
				 I2C_EE_WaitEepromStandbyState();
			 }
		 }
	 }
	 /* If WriteAddr is not I2C_PageSize aligned  */
	 else 
	 {
		 /* If NumByteToWrite < I2C_PageSize */
		 if(NumOfPage== 0) 
		 {
			 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			 I2C_EE_WaitEepromStandbyState();
		 }
		 /* If NumByteToWrite > I2C_PageSize */
		 else
		 {
			 NumByteToWrite -= count;
			 NumOfPage =  NumByteToWrite / I2C_PageSize;
			 NumOfSingle = NumByteToWrite % I2C_PageSize;	

			 if(count != 0)
			 {  
				 I2C_EE_PageWrite(pBuffer, WriteAddr, count);
				 I2C_EE_WaitEepromStandbyState();
				 WriteAddr += count;
				 pBuffer += count;
			 } 

			 while(NumOfPage--)
			 {
				 I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
				 I2C_EE_WaitEepromStandbyState();
				 WriteAddr +=  I2C_PageSize;
				 pBuffer += I2C_PageSize;  
			 }
			 if(NumOfSingle != 0)
			 {
				 I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
				 I2C_EE_WaitEepromStandbyState();
			 }
		 }
	 }  
 }

 void I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

 void I2C_Test(void)
 {
	 u16 i;
	 u8 I2c_Buf_Write[256];
	 u8 I2c_Buf_Read[256];

	 printf("д�������\r\n");

	 for(i=0;i<=255;i++) //��仺��
	 {   
		 I2c_Buf_Write[i]=i;
		 printf("0x%02X ",I2c_Buf_Write[i]);
		 if(i%16 == 15)
		 {
			 printf("\r\n");
		 }
	 }

	 //��I2c_Buf_Write��˳�����������д��EERPOM�� 
	 I2C_EE_BufferWrite(I2c_Buf_Write,EEP_Firstpage,256);	 

	 printf("\r\n����������\r\n");
	 //��EEPROM��������˳�򱣱��浽I2c_Buf_Read�� 
	 I2C_EE_BufferRead(I2c_Buf_Read,EEP_Firstpage,256); 

	 //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
	 for(i=0;i<256;i++)
	 {	
		 if(I2c_Buf_Read[i]!=I2c_Buf_Write[i])
		 {
			 printf("0x%02X ", I2c_Buf_Read[i]);
			 printf("����:I2C EEPROMд������������ݲ�һ��\r\n");
			 return;
		 }
		 printf("0x%02X ", I2c_Buf_Read[i]);
		 if(i%16 == 15)
		 {
			 printf("\r\n");
		 }
	 }
	 printf("��д����ͨ��PASSED\n\r");



 }
