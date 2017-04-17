					 /**
  ******************************************************************************
  * @file    stm3210e_eval_lcd.c
  * @author  ARMJISHU Application Team
  * @version 
  * @date    
  * @brief   This file includes the LCD driver for AM-240320L8TNQW00H 
  *          (LCD_ILI9320) and AM-240320LDTNQW00H (LCD_SPFD5408B) Liquid Crystal
  *          Display Module of STM3210E-EVAL board.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_fsmc.h"
#include "ili9320.h"
#include "ili9320_font.h"
#include <stdio.h>

u16 DeviceCode;

extern void delay_ms(u16 nms);
/* ��ʾ�ַ�ʱ͸������ģʽ���� */
#define HyalineBackEn           0x01  //  ͸������
#define HyalineBackDis          0x00  //  ��͸������(Ĭ��)

static __IO uint8_t HyalineBack = HyalineBackDis;

/* ǰ��ɫTextColor�ͱ���ɫBackColor */
static  __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

const u8 WelcomeStr[13][72]={
    //�ﻶӭ��ʹ������ϵ�п�����
    //No:0	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x18,0x00,0x00,0x18,0x00,0x00,
    0x18,0x00,0x00,0x3C,0x00,0x00,0x3C,0x00,0x00,0x7C,0x00,0x3F,0xFF,0xFC,0x1F,0xFF,0xF8,0x07,0xFF,
    0xE0,0x03,0xFF,0xC0,0x01,0xFF,0x80,0x01,0xFF,0x80,0x01,0xFF,0x80,0x01,0xFF,0x80,0x03,0xE7,0xC0,
    0x03,0x81,0xC0,0x03,0x00,0xC0,0x04,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00},
    //No:1	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x06,0x00,0x00,0x46,0x00,0x3F,0xC4,0x00,0x00,
    0xC4,0x04,0x00,0x8F,0xFE,0x20,0x88,0x08,0x11,0x89,0x90,0x09,0x11,0x80,0x05,0x21,0x80,0x02,0x21,
    0x80,0x03,0x03,0x80,0x05,0x82,0x80,0x05,0x82,0x40,0x08,0xC2,0x40,0x08,0xC4,0x60,0x10,0x4C,0x20,
    0x20,0x08,0x30,0x40,0x10,0x18,0x00,0x60,0x0E,0x00,0x80,0x00,0x00,0x00,0x00},
    //No:2	ӭ   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x02,0x00,0x0C,0x0E,0x00,0x06,0x72,0x08,0x04,0x41,0xFC,0x00,
    0x41,0x08,0x00,0x41,0x08,0x04,0x41,0x08,0x7E,0x41,0x08,0x04,0x41,0x08,0x04,0x41,0x08,0x04,0x41,
    0x08,0x04,0x45,0x08,0x04,0x59,0x08,0x04,0x61,0x78,0x04,0x41,0x18,0x04,0x01,0x00,0x1A,0x01,0x00,
    0x71,0x00,0x00,0x60,0xE0,0x02,0x00,0x3F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00},
    //No:3	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x18,0x00,0x06,0x18,0x00,0x04,0x30,0x00,0x0C,0x3F,0xFC,0x0C,
    0x44,0x10,0x14,0x43,0x20,0x24,0x82,0x00,0x44,0x32,0x40,0x04,0x22,0x20,0x04,0x42,0x18,0x04,0x82,
    0x18,0x05,0x1E,0x08,0x04,0x06,0x00,0x00,0x20,0x00,0x01,0x98,0x20,0x09,0x8C,0x10,0x09,0x88,0x4C,
    0x19,0x80,0x4C,0x31,0x80,0x44,0x01,0x80,0xE0,0x00,0xFF,0xC0,0x00,0x00,0x00},
    //No:4	ʹ   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x81,0x00,0x03,0x01,0x00,0x03,0x01,0x04,0x02,0xFF,0xFE,0x06,
    0x01,0x00,0x04,0x01,0x00,0x0E,0x21,0x18,0x0E,0x3F,0xE8,0x16,0x21,0x08,0x26,0x21,0x08,0x26,0x21,
    0x08,0x46,0x3F,0xF8,0x06,0x21,0x00,0x06,0x13,0x00,0x06,0x12,0x00,0x06,0x0A,0x00,0x06,0x06,0x00,
    0x06,0x06,0x00,0x06,0x0F,0x00,0x06,0x10,0xE0,0x06,0x60,0x3C,0x01,0x80,0x00},
    //No:5	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x10,0x0F,0xFF,0xF8,0x0C,0x08,0x10,0x0C,0x08,0x10,0x0C,
    0x08,0x10,0x0C,0x08,0x10,0x0C,0x08,0x10,0x0F,0xFF,0xF0,0x08,0x08,0x10,0x08,0x08,0x10,0x08,0x08,
    0x10,0x08,0x08,0x10,0x0F,0xFF,0xF0,0x08,0x08,0x10,0x08,0x08,0x10,0x08,0x08,0x10,0x10,0x18,0x10,
    0x10,0x18,0x10,0x20,0x18,0x10,0x20,0x18,0xF0,0x40,0x00,0x30,0x00,0x00,0x00},
    //No:6	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x01,0x00,0x04,0x00,0x80,0x02,0x00,0x80,0x02,0x00,0x80,0x00,0x00,0x88,0x00,
    0x9F,0xFC,0x3F,0x90,0x88,0x01,0x10,0x88,0x03,0x10,0x88,0x02,0x1F,0xF8,0x06,0x10,0x88,0x0D,0x90,
    0x88,0x0C,0xD0,0x88,0x14,0x50,0x8C,0x24,0x1F,0xFC,0x44,0x10,0x88,0x04,0x00,0x80,0x04,0x00,0x80,
    0x04,0x00,0x80,0x04,0x00,0x80,0x04,0x00,0x80,0x04,0x01,0x80,0x00,0x01,0x00},
    //No:7	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x10,0x00,0x00,0x20,0x00,0x03,0xFF,0xE0,0x02,
    0x00,0x40,0x02,0x20,0x40,0x02,0x10,0x40,0x02,0x18,0x40,0x02,0x08,0x40,0x02,0x00,0x40,0x02,0x00,
    0x4E,0x3F,0xFF,0xF0,0x02,0x00,0x40,0x02,0x20,0x40,0x02,0x10,0x40,0x06,0x18,0x40,0x06,0x18,0x40,
    0x04,0x00,0x40,0x08,0x00,0x40,0x10,0x07,0xC0,0x20,0x01,0x80,0x40,0x00,0x00},
    //No:8	ϵ   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x7F,0x00,0x1F,0x98,0x00,0x00,0x30,0x00,0x00,
    0x40,0x40,0x00,0x80,0xC0,0x07,0x3F,0x00,0x07,0xC4,0x00,0x00,0x18,0x00,0x00,0x60,0x40,0x00,0x80,
    0x20,0x07,0x7F,0xF8,0x07,0x88,0x18,0x01,0x08,0x00,0x00,0xC8,0x80,0x01,0x88,0x60,0x02,0x08,0x38,
    0x04,0x08,0x1C,0x18,0x08,0x0C,0x20,0xF8,0x0C,0x00,0x18,0x00,0x00,0x00,0x00},
    //No:9	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x0C,0x08,0x3F,0xF0,0x08,0x01,0x00,0x08,0x03,
    0x01,0x88,0x02,0x00,0x88,0x02,0x18,0x88,0x07,0xF8,0x88,0x04,0x10,0x88,0x0C,0x30,0x88,0x0A,0x20,
    0x88,0x13,0x20,0x88,0x21,0x60,0x88,0x01,0x40,0x88,0x00,0xC0,0x88,0x00,0x81,0x88,0x01,0x01,0x08,
    0x02,0x00,0x08,0x04,0x00,0x08,0x18,0x00,0x78,0x20,0x00,0x18,0x00,0x00,0x00},
    //No:10	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x1F,0xFF,0xE0,0x00,0x81,0x80,0x00,
    0x81,0x80,0x00,0x81,0x80,0x00,0x81,0x80,0x00,0x81,0x80,0x00,0x81,0x84,0x00,0x81,0x8C,0x1F,0xFF,
    0xF0,0x00,0x81,0x80,0x00,0x81,0x80,0x00,0x81,0x80,0x01,0x81,0x80,0x01,0x01,0x80,0x01,0x01,0x80,
    0x02,0x01,0x80,0x04,0x01,0x80,0x08,0x01,0x80,0x30,0x01,0x80,0x00,0x00,0x00},
    //No:11	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x00,0x04,0x60,0xC0,0x0C,0x60,0x60,0x0C,0x60,0x60,0x08,
    0x40,0x00,0x18,0x40,0x0C,0x17,0xFF,0xF0,0x00,0xC0,0x00,0x00,0xC0,0x00,0x00,0xFF,0xE0,0x00,0xA0,
    0x60,0x01,0xA0,0x40,0x01,0x10,0x80,0x03,0x09,0x80,0x02,0x09,0x00,0x06,0x06,0x00,0x04,0x06,0x00,
    0x08,0x0D,0x80,0x10,0x30,0xE0,0x20,0x40,0x3E,0x43,0x80,0x08,0x00,0x00,0x00},
    //No:12	��   ʹ��Ƶ��=1
    {	0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x30,0x04,0x00,0xF8,0x04,0x1F,0x00,0x04,0x10,0x00,0x04,
    0x90,0x00,0x3F,0x10,0x00,0x0C,0x10,0x10,0x0C,0x1F,0xF0,0x0E,0x14,0x30,0x0D,0x14,0x20,0x1C,0x94,
    0x20,0x14,0xB2,0x60,0x14,0x32,0x40,0x24,0x23,0xC0,0x44,0x21,0x80,0x44,0x21,0x80,0x04,0x23,0xC0,
    0x04,0x46,0x60,0x04,0x88,0x38,0x04,0xB0,0x1C,0x05,0xC0,0x00,0x00,0x00,0x00},
};

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE   ((u32)(0x60000000 | 0x0C000000))
#define LCD         ((LCD_TypeDef *) LCD_BASE)

//#define SetCs  
//#define ClrCs  

//#define SetCs  GPIO_SetBits(GPIOG, GPIO_Pin_12);
//#define ClrCs  GPIO_ResetBits(GPIOG, GPIO_Pin_12);

//#define SetCs  GPIO_SetBits(GPIOB, GPIO_Pin_8);
//#define ClrCs  GPIO_ResetBits(GPIOB, GPIO_Pin_8);

////////////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);
  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate 
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /*FSMC A21��A22���Ի������츴�����*/ 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
  GPIO_Init(GPIOE, &GPIO_InitStructure); 

 

  /* LEDs pins configuration */
 
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  Timing_read, Timing_write;

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 4 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  Timing_read.FSMC_AddressSetupTime = 6;             
  Timing_read.FSMC_AddressHoldTime = 0;  
  Timing_read.FSMC_DataSetupTime = 6; 
  Timing_read.FSMC_BusTurnAroundDuration = 0;
  Timing_read.FSMC_CLKDivision = 0;
  Timing_read.FSMC_DataLatency = 0;
  Timing_read.FSMC_AccessMode = FSMC_AccessMode_A;    

  Timing_write.FSMC_AddressSetupTime = 2;             
  Timing_write.FSMC_AddressHoldTime = 0;  
  Timing_write.FSMC_DataSetupTime = 2; 
  Timing_write.FSMC_BusTurnAroundDuration = 0;
  Timing_write.FSMC_CLKDivision = 0;
  Timing_write.FSMC_DataLatency = 0;  
  Timing_write.FSMC_AccessMode = FSMC_AccessMode_A; 
   
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */
  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &Timing_read;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &Timing_write;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  

  /* BANK 4 (of NOR/SRAM Bank 1~4) is enabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
     
}


void LCD_X_Init(void)
{
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();

/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  //ClrCs	 //���ClrCS ����û���á�
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;
 // SetCs	//��� SetCs �� ������ʲô��
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
  u16 data;	  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;	 
  data = LCD->LCD_RAM; 
  return    data;
}

u16 LCD_ReadSta(void)
{
  u16 data;
  /* Write 16-bit Index, then Write Reg */
 // ClrCs
  data = LCD->LCD_REG;
 // SetCs
  return    data;
}

void LCD_WriteCommand(u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
 // ClrCs
  LCD->LCD_REG = LCD_RegValue;
  //SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
  //ClrCs
  LCD->LCD_REG = R34;//0x22
 // SetCs
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)					 
{
 // ClrCs
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
 // SetCs
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
* Input          : None
* Output         : None
* Return         : LCD RAM Value.
*******************************************************************************/
u16 LCD_ReadRAM(void)
{
  u16 dummy;
  u16 data;
  /* Write 16-bit Index (then Read Reg) */
  //ClrCs
  LCD->LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD->LCD_RAM; 
  dummy++;
  data = LCD->LCD_RAM; 
  //SetCs
  return    data;
  //return LCD->LCD_RAM;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/


void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
  LCD_WriteReg(0x06,Ypos>>8);
  LCD_WriteReg(0x07,Ypos);
  
  LCD_WriteReg(0x02,Xpos>>8);
  LCD_WriteReg(0x03,Xpos);  
}			 

// ����� ���� ��ʱ ���� ̫�ͼ��ˡ�

//void Delay_nms(int n)
//{
//  
//  u32 f=n,k;
//  for (; f!=0; f--)
//  {
//    for(k=0xFFF; k!=0; k--);
//  }
//  
//}
//
//void Delay(u32 nCount)
//{
// u32 TimingDelay; 
// while(nCount--)
//   {
//    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
//   }
//}

/**
  * @brief  Draws a chinacharacter on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChinaChar(u8 Xpos, u16 Ypos, const u8 *c)
{
  u32 index = 0, i = 0, j = 0;
  u8 Xaddress = 0;
   
  Xaddress = Xpos;
  
  //ili9320_SetCursor(Xaddress, Ypos);
  ili9320_SetCursor(Ypos,Xaddress);

  for(index = 0; index < 24; index++)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(j = 0; j < 3; j++)
    {
        for(i = 0; i < 8; i++)
        {
          if((c[3*index + j] & (0x80 >> i)) == 0x00)
          {
              LCD_WriteRAM(0xF800);
          }
          else
          {
            LCD_WriteRAM(0xFFE0);
          }
        }   
     }
    Xaddress++;
    ili9320_SetCursor(Ypos, Xaddress);
  }
}

void LCD_DisplayWelcomeStr(u8 Line)
{
  u16 num = 0;

  /* Send the string character by character on LCD */
  for(num=0; num<13; num++)
  {
    /* Display one China character on LCD */
    LCD_DrawChinaChar(Line, num*24+4, (u8 *)WelcomeStr[num]);
  }
}
/****************************************************************************
* ��    �ƣ�void ili9320_Initializtion()
* ��    �ܣ���ʼ�� ����TFT LCDҺ�����Ŀ�����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Initializtion();
****************************************************************************/
void ili9320_Initializtion(void)
{
   u16 i;
   LCD_X_Init();  //��ʼ������LCD��ɫҺ�����ϵĹܽţ������������ӵ���FSMC����
   delay_ms(50);
   LCD_WriteReg(0x0000,0x0001);  
     delay_ms(50);/* delay 50 ms */			//start internal osc
   DeviceCode = LCD_ReadReg(0x0000);
   delay_ms(50); /* delay 50 ms */
   DeviceCode = LCD_ReadReg(0x0000);  
	
   if(DeviceCode==0x9320||DeviceCode==0x9300)
   {
		LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		//LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.
			LCD_WriteReg(0x03, 0x1030);
		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.
	
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
	
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));					//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.
	
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));
		
		LCD_WriteReg(0x50,0);		//Set X Start.
		LCD_WriteReg(0x51,239);	//Set X End.
		LCD_WriteReg(0x52,0);		//Set Y Start.
		LCD_WriteReg(0x53,319);	//Set Y End.
	
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
	
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.	
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}
	else if(DeviceCode==0x9331)
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit   0x0100
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
		LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.     0x1030
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		ili9320_Delay(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x0000); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);		
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0061);
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0133);  // 262K color and display ON
		ili9320_Delay(50);
	}
	else if(DeviceCode==0x8989)
	{   
    	LCD_WriteReg(0x0000,0x0001);   //�򿪾���         
    	LCD_WriteReg(0x0010,0x0000);                     
    	   delay_ms(50);; // Wait 30ms            
    	LCD_WriteReg(0x0007,0x0233);                
		LCD_WriteReg(0x0011,0x6078); //�������ݸ�ʽ  16λɫ 		���� 0x6058           
    	LCD_WriteReg(0x0002,0x0600);       
    	LCD_WriteReg(0x0003,0xA8A4);//0x0804  
   		LCD_WriteReg(0x000C,0x0000);
    	LCD_WriteReg(0x000D,0x080C);       
    	LCD_WriteReg(0x000E,0x2900);       
    	LCD_WriteReg(0x001E,0x00B8);       
    	LCD_WriteReg(0x0001,0x293F);
    	LCD_WriteReg(0x0010,0x0000);       
    	LCD_WriteReg(0x0005,0x0000);       
    	LCD_WriteReg(0x0006,0x0000);       
  		LCD_WriteReg(0x0016,0xEF1C);     
		LCD_WriteReg(0x0017,0x0003);     
		LCD_WriteReg(0x0007,0x0233);		//0x0233       
    	LCD_WriteReg(0x000B,0x0000|(3<<6));     
		LCD_WriteReg(0x000F,0x0000);		//ɨ�迪ʼ��ַ
	  	LCD_WriteReg(0x0041,0x0000);     
	  	LCD_WriteReg(0x0042,0x0000);     
	  	LCD_WriteReg(0x0048,0x0000);     
	  	LCD_WriteReg(0x0049,0x013F);     
	  	LCD_WriteReg(0x004A,0x0000);     
	  LCD_WriteReg(0x004B,0x0000);     
	  LCD_WriteReg(0x0044,0xEF00);     
	  LCD_WriteReg(0x0045,0x0000);     
	  LCD_WriteReg(0x0046,0x013F);     
	  LCD_WriteReg(0x0030,0x0707);     
	  LCD_WriteReg(0x0031,0x0204);     
	  LCD_WriteReg(0x0032,0x0204);     
	  LCD_WriteReg(0x0033,0x0502);     
	  LCD_WriteReg(0x0034,0x0507);     
	  LCD_WriteReg(0x0035,0x0204);     
	  LCD_WriteReg(0x0036,0x0204);     
	  LCD_WriteReg(0x0037,0x0502);     
	  LCD_WriteReg(0x003A,0x0302);     
	  LCD_WriteReg(0x003B,0x0302);     
	  LCD_WriteReg(0x0023,0x0000);     
	  LCD_WriteReg(0x0024,0x0000);     
	  LCD_WriteReg(0x0025,0x8000);     
	  LCD_WriteReg(0x004e,0);        //��(X)��ַ0
	  LCD_WriteReg(0x004f,0);        //��(Y)��ַ0
	}
	else if(DeviceCode==0x9325||DeviceCode==0x9328)
	{
  		LCD_WriteReg(0x00e7,0x0010);      
        LCD_WriteReg(0x0000,0x0001);  			//start internal osc
        LCD_WriteReg(0x0001,0x0100);     
        LCD_WriteReg(0x0002,0x0700); 				//power on sequence                     
        LCD_WriteReg(0x0003,(1<<12)|(1<<3)); 	//65K 
        LCD_WriteReg(0x0004,0x0000);                                   
        LCD_WriteReg(0x0008,0x0207);	           
        LCD_WriteReg(0x0009,0x0000);         
        LCD_WriteReg(0x000a,0x0000); 				//display setting         
        LCD_WriteReg(0x000c,0x0001);				//display setting          
        LCD_WriteReg(0x000d,0x0000); 				//0f3c          
        LCD_WriteReg(0x000f,0x0000);
//Power On sequence //
        LCD_WriteReg(0x0010,0x0000);   
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);                                                                 
        LCD_WriteReg(0x0013,0x0000);                 
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0010,0x1590);   
        LCD_WriteReg(0x0011,0x0227);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0012,0x009c);                  
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0013,0x1900);   
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);           
///////////////////////////////////////////////////////      
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0030,0x0007); 
        LCD_WriteReg(0x0031,0x0707);   
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04); 
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);        
        LCD_WriteReg(0x0039,0x0706);     
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0050,0x0000);        
        LCD_WriteReg(0x0051,0x00ef);   
        LCD_WriteReg(0x0052,0x0000);     
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);        
        LCD_WriteReg(0x0061,0x0001); 
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
      
        LCD_WriteReg(0x0090,0x0010);     
        LCD_WriteReg(0x0092,0x0000);  
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);        
        LCD_WriteReg(0x0098,0x0000);  
         //display on sequence     
        LCD_WriteReg(0x0007,0x0133);
    
        LCD_WriteReg(0x0020,0x0000);                                                            
        LCD_WriteReg(0x0021,0x0000);
	}else
    {
       printf("\n\r ###### Err: Unknow DeviceCode 0x%x ###### ", DeviceCode);
    }
	ili9320_Clear(Green);
    Lcd_Light_ON  
}

/****************************************************************************
* ��    �ƣ�void ili9320_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetCursor(10,10);
****************************************************************************/
//inline void ili9320_SetCursor(u16 x,u16 y)
void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceCode==0x8989)
	{
	 	LCD_WriteReg(0x004e,x);        //��
    	LCD_WriteReg(0x004f,y);  //��
	}
	else if((DeviceCode==0x9919))
	{
		LCD_WriteReg(0x004e,x); // ��
  		LCD_WriteReg(0x004f,y); // ��	
	}
//	else if((DeviceCode==0x9325))
//	{
//		LCD_WriteReg(0x0020,y); // ��
//		LCD_WriteReg(0x0021,x); // ��	
//	}
	else
	{
  		LCD_WriteReg(0x0020,x); // ��
  		LCD_WriteReg(0x0021,0x13f-y); // ��
	}
}

/****************************************************************************
* ��    �ƣ�void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* ��    �ܣ����ô�������
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetWindows(0,0,100,100)��
****************************************************************************/
//inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}

/****************************************************************************
* ��    �ƣ�void ili9320_Clear(u16 dat)
* ��    �ܣ�����Ļ����ָ������ɫ��������������� 0xffff
* ��ڲ�����dat      ���ֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 Color)
{
  u32 index=0;
  ili9320_SetCursor(0,0); 
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)	 //��� 76800 ��ʾ 320*240�� ������GRAM�� ��ַ���Զ���һ��
  {
     LCD->LCD_RAM=Color;
  }
}

/****************************************************************************
* ��    �ƣ�u16 ili9320_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
* ˵    ����
* ���÷�����i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{
  ili9320_SetCursor(x,y);
  return (ili9320_BGR2RGB(LCD_ReadRAM()));
}
/****************************************************************************
* ��    �ƣ�void ili9320_SetPoint(u16 x,u16 y,u16 point)
* ��    �ܣ���ָ�����껭��
* ��ڲ�����x      ������
*           y      ������
*           point  �����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  if ( (x>320)||(y>240) ) return;
  ili9320_SetCursor(x,y);

  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}


/****************************************************************************
* ��    �ƣ�void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* ��    �ܣ���ָ�����귶Χ��ʾһ��ͼƬ
* ��ڲ�����StartX     ����ʼ����
*           StartY     ����ʼ����
*           EndX       �н�������
*           EndY       �н�������
            pic        ͼƬͷָ��
* ���ڲ�������
* ˵    ����ͼƬȡģ��ʽΪˮƽɨ�裬16λ��ɫģʽ
* ���÷�����ili9320_DrawPicture(0,0,100,100,(u16*)demo);
* ��    �ߣ� www.armjishu.com
****************************************************************************/
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
{
  u32  i, total;
  u16 data1,data2,data3;
  u16 *picturepointer = pic;
  u16 x,y;
  
  printf("ili9320_DrawPicture StartX %d StartY %d EndX %d EndY %d \n\r", StartX, StartY, EndX, EndY);

  x=StartX;
  y=StartY;
  
  total = (EndX - StartX + 1)*(EndY - StartY + 1 )/2;

  for (i=0;i<total;i++)
  {
      data1 = *picturepointer++;
      data2 = *picturepointer++;
      data3 = ((data1 >>3)& 0x001f) |((data1>>5) & 0x07E0) | ((data2<<8) & 0xF800);
      ili9320_SetPoint(x,y,data3);
      y++;
      if(y > EndY)
      {
          x++;
          y=StartY;
      }


      data1 = data2;
      data2 = *picturepointer++;
      data3 = ((data1 >>11)& 0x001f) |((data2<<3) & 0x07E0) | ((data2) & 0xF800);
      ili9320_SetPoint(x,y,data3);
      y++;
      if(y > EndY)
      {
          x++;
          y=StartY;
      }
  }

}

/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)  // Lihao
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;
  
  if(HyalineBackColor == bkColor)
  {
    for (i=0;i<16;i++)
    {
      tmp_char=ascii_8x16[((c-0x20)*16)+i];
      for (j=0;j<8;j++)
      {
        if ( (tmp_char >> 7-j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            // do nothing // ͸������
          }
      }
    }
  }
  else
  {
        for (i=0;i<16;i++)
    {
      tmp_char=ascii_8x16[((c-0x20)*16)+i];
      for (j=0;j<8;j++)
      {
        if ( (tmp_char >> 7-j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
          }
      }
    }
  }
}

/****************************************************************************
* ��    �ƣ�void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* ��    �ܣ���ָ��������ʾһ��8x16�����ascii�ַ�
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar_16x24(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{

  u16 i=0;
  u16 j=0;
  
  u16 tmp_char=0;

  if(HyalineBackColor == bkColor)
  {
    for (i=0;i<24;i++)
    {
      tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
      for (j=0;j<16;j++)
      {
        if ( (tmp_char >> j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
              // do nothing // ͸������
          }
      }
    }
  }
  else
  {
    for (i=0;i<24;i++)
    {
      tmp_char=ASCII_Table_16x24[((c-0x20)*24)+i];
      for (j=0;j<16;j++)
      {
        if ( (tmp_char >> j) & 0x01 == 0x01)
          {
            ili9320_SetPoint(x+j,y+i,charColor); // �ַ���ɫ
          }
          else
          {
            ili9320_SetPoint(x+j,y+i,bkColor); // ������ɫ
          }
      }
    }
  }
}
/****************************************************************************
* ��    �ƣ�u16 ili9320_BGR2RGB(u16 c)
* ��    �ܣ�RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
* ��ڲ�����c      BRG ��ɫֵ
* ���ڲ�����RGB ��ɫֵ
* ˵    �����ڲ���������
* ���÷�����
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b, rgb;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);

  return( rgb );
}

/****************************************************************************
* ��    �ƣ�void ili9320_BackLight(u8 status)
* ��    �ܣ�������Һ������
* ��ڲ�����status     1:���⿪  0:�����
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    Lcd_Light_ON;
  }
  else
  {
    Lcd_Light_OFF;
  }
}

/****************************************************************************
* ��    �ƣ�void ili9320_Delay(vu32 nCount)
* ��    �ܣ���ʱ
* ��ڲ�����nCount   ��ʱֵ
* ���ڲ�������
* ˵    ����
* ���÷�����ili9320_Delay(1000);
****************************************************************************/
void ili9320_Delay(vu32 nCount)
{
  // Delay(nCount);
	delay_ms(nCount);
  //for(; nCount != 0; nCount--);
}


//  if ( (x>320)||(y>240) ) return;
//  ili9320_SetCursor(x,y);
//
//  LCD_WriteRAM_Prepare();
//  LCD_WriteRAM(point);
/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *cpFontArray)
{
    uint32_t index = 0, i = 0, j = 0, k = 0, y;
    uint8_t Xaddress = 0;
/**************** Armjishu.com Add { *************************************/
	uint32_t  Width = 24;
	uint32_t  Height = 48;
	uint32_t  BytesPreChar = 144;
/**************** Armjishu.com Add } *************************************/	 
    Xaddress = Xpos;

    ili9320_SetCursor(Xaddress, Ypos);

    for(y = 0; y < Height; y++)
    {
        LCD_WriteRAM_Prepare(); 
        for(i = Width; i > 0;)
        {
            if(i>=8)
            {
                k = 8;
            }
            else
            {
                k = i;
            }

            for(j = 0; j < k; j++)
            {
                if((cpFontArray[index] & (0x80 >> j)) == 0x00)
                {
                    if(HyalineBack == HyalineBackDis)
                    {
                        LCD_WriteRAM(BackColor); //putchar(' ');
                    }
                    else
                    {
                        LCD_SetCursor(Xaddress, Ypos+(Width - i)+1);
                        LCD_WriteRAM_Prepare();
                    }
                }
                else
                {
                    LCD_WriteRAM(TextColor); //putchar('*');
                }
                i--;
                //Delay(2);//�˴����ӳٿ�������Ŀ�������ַ���һ�����ĵ���ƹ���
            }
            index++;
        }   
        //SetCs
        Xaddress++;
        ili9320_SetCursor(Xaddress, Ypos);
    }
    //putchar('\r');putchar('\n');
}
