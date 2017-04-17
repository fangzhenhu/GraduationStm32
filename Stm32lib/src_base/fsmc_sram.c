/* Includes ------------------------------------------------------------------*/
#include "fsmc_sram.h"
#include <stm32f10x_fsmc.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define Bank1_SRAM3_ADDR    ((u32)0x68000000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : FSMC_SRAM_Init
* Description    : Configures the FSMC and GPIOs to interface with the SRAM memory.
*                  This function must be called before any write/read operation
*                  on the SRAM.
* Input          : None 
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;
  	GPIO_InitTypeDef GPIO_InitStructure; 
    //首先我觉得因该用到的引脚：PF,PG ,PD,PE
	/*使能FSMC时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE); //使能FSMC时钟
	/*FSMC总线使用的GPIO组时钟使能*/
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG | RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF, ENABLE);

    /*FSMC数据线FSMC_D[0:15]初始化，推挽复用输出*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
  
    /*FSMC地址线FSMC_A[0:17]初始化，推挽复用输出*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | 
                                  GPIO_Pin_14 | GPIO_Pin_15;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
                                  GPIO_Pin_4 | GPIO_Pin_5;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13; 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
   
  	/*FSMC NOE和NWE初试化，推挽复用输出*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/*FSMC NE3初试化，推挽复用输出*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
  
  	/*FSMC NBL0和NBL1初试化，推挽复用输出*/  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure); 
  
/*--------------FSMC 总线 存储器参数配置------------这些参数 真不好理解？东西还是学少了！------------------*/
  	readWriteTiming.FSMC_AddressSetupTime = 0;           //地址建立时间    
  	readWriteTiming.FSMC_AddressHoldTime = 0;            //地址保持时间  
  	readWriteTiming.FSMC_DataSetupTime = 2;              //数据建立时间
  	readWriteTiming.FSMC_BusTurnAroundDuration = 0;      //总线恢复时间
  	readWriteTiming.FSMC_CLKDivision = 0;                // 时钟分频因子 
  	readWriteTiming.FSMC_DataLatency = 0;          	   //数据产生时间
  	readWriteTiming.FSMC_AccessMode =  FSMC_AccessMode_A; //FSMC NOR控制器时序
  	
/*--------------FSMC 总线 参数配置------------------------------*/
  	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;                  //使用了FSMC的BANK1的子板块3             
  	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//禁止地址数据线复用
  	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;           //存储器类型为SRAM
  	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;  //存储器数据宽度为16位
  	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; //关闭突发模式访问
    //等待信号优先级，只有在使能突发访问模式才有效
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low; 
    //关闭Wrapped burst access mode，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;         
    //等待信号设置，只有在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;     //使能这个BANK的写操作
    //使能/关闭等待信息设置，只在使能突发访问模式才有效
  	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;     
  	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; //关闭Extend Mode
  	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;     //关闭Write Burst Mode   
  	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;               //读操作时序参数
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;                   //写操作时序参数

  	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

/*--------------使能BANK1的子板块3------------------------------*/
  	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  
}

//在指定地址开始,连续写入n个字节.
//pBuffer:字节指针
//WriteAddr:要写入的地址
//n:要写入的字节数
void FSMC_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite)
{
  for(; NumHalfwordToWrite != 0; NumHalfwordToWrite--) /* while there is data to write */
  {
    /* Transfer data to the memory */
    *(u16 *) (Bank1_SRAM3_ADDR + WriteAddr) = *pBuffer++;
    
    /* Increment the address*/  
    WriteAddr += 2;//这里需要加2，是因为STM32的FSMC地址右移一位对其.加2相当于加1.
  }   
}

/*******************************************************************************
* Function Name  : FSMC_SRAM_ReadBuffer
* Description    : Reads a block of data from the FSMC SRAM memory.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the SRAM memory.
*                  - ReadAddr : SRAM memory internal address to read from.
*                  - NumHalfwordToRead : number of half-words to read.
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead)
{
  for(; NumHalfwordToRead != 0; NumHalfwordToRead--) /* while there is data to read */
  {
    /* Read a half-word from the memory */
    *pBuffer++ = *(vu16*) (Bank1_SRAM3_ADDR + ReadAddr);

    /* Increment the address*/  
    ReadAddr += 2;
  }  
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
