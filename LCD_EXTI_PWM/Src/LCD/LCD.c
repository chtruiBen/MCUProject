/*********************************************************************************************************
*
* File                : LCD.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LCD.h"

/* Private define ------------------------------------------------------------*/
#define LCD_BASE   (0x60000000UL | 0x0C000000UL)
#define LCD_REG              (*((volatile unsigned short *) 0x6F000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x6F000800)) /* RS = 1 */  //100 0000 0000

/* Private variables ---------------------------------------------------------*/
static uint8_t LCD_Code;

/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */	   
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */	
#define  LGDP4535   12 /* 0x4535 */  
#define  SSD2119    13 /* 3.5 LCD 0x9919 */
#define ILI9341 	  14

void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	uint16_t xsize,ysize;
	
	if(LCD_Code==ILI9341)//9341/6804/5310/5510,������
	{
		dir=D2U_L2R;
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
	
		 dirreg=0X36;
    regval|=0X08;//5310/5510����ҪBGR   
 
		LCD_WriteReg(dirreg,regval);
 		
			LCD_WR_REG(0x2A); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((320-1)>>8);LCD_WR_DATA((320-1)&0XFF);
			LCD_WR_REG(0x2B); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((240-1)>>8);LCD_WR_DATA((240-1)&0XFF);  
	printf("z\r\n");
  	}

}     
void LCD_Display_Dir(uint8_t dir)
{
	
		
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 

/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_CtrlLinesConfig(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_FMC_CLK_ENABLE();  
	__HAL_RCC_GPIOD_CLK_ENABLE(); 
	__HAL_RCC_GPIOE_CLK_ENABLE(); 
	__HAL_RCC_GPIOG_CLK_ENABLE(); 
	__HAL_RCC_GPIOB_CLK_ENABLE(); 

	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|\
		GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;                 
		GPIO_Initure.Pull=GPIO_PULLUP;                        
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;                
		GPIO_Initure.Alternate=GPIO_AF12_FMC;        
		HAL_GPIO_Init(GPIOD,&GPIO_Initure);    

		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
		GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);

		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_12;
		HAL_GPIO_Init(GPIOG,&GPIO_Initure);
		/*LCD BL*/
		GPIO_Initure.Pin=GPIO_PIN_0;            
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
		GPIO_Initure.Pull=GPIO_PULLUP;          
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;  
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);		
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);

//	GPIO_InitTypeDef GPIO_InitStructure; 
//	
//	/* Enable GPIOs clock */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG, ENABLE);
//	
//	/* Enable FSMC clock */
//	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 

//	/*-- GPIOs Configuration ------------------------------------------------------*/
//	/*
//	+-------------------+--------------------+------------------+------------------+
//	+                       SRAM pins assignment                                   +
//	+-------------------+--------------------+------------------+------------------+
//	| PD0  <-> FSMC_D2  | PE2  <-> FSMC_A23  | PG5 <-> FSMC_A15	|
//	| PD1  <-> FSMC_D3  |                    | PG9 <-> FSMC_NE2 |
//	| PD4  <-> FSMC_NOE | PE7  <-> FSMC_D4   | PG13<-> FSMC_A24 |
//	| PD5  <-> FSMC_NWE | PE8  <-> FSMC_D5   |------------------+ 
//	| PD8  <-> FSMC_D13 | PE9  <-> FSMC_D6   | 
//	| PD9  <-> FSMC_D14 | PE10 <-> FSMC_D7   |  
//	| PD10 <-> FSMC_D15 | PE11 <-> FSMC_D8   | 
//	|                   | PE12 <-> FSMC_D9   | 
//	|                   | PE13 <-> FSMC_D10  |                   
//	| PD14 <-> FSMC_D0  | PE14 <-> FSMC_D11  |                  
//	| PD15 <-> FSMC_D1  | PE15 <-> FSMC_D12  |
//	+-------------------+--------------------+
//	*/
//	/* GPIOD configuration */
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | 
//	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	
//	
//	/* GPIOE configuration */
//	//GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
//	
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
//	                             GPIO_Pin_11| GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	

//	/* GPIOG configuration */
//	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FSMC);
//	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_FSMC);
//	//GPIO_PinAFConfig(GPIOG, GPIO_PinSource13 , GPIO_AF_FSMC);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;      
//	
//	GPIO_Init(GPIOG, &GPIO_InitStructure);
//	
//	/*LCD BL*/
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIOB->BSRRL = GPIO_Pin_0;
}

/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
SRAM_HandleTypeDef SRAM_Handler; 
static void LCD_FSMCConfig(void)
{
	 FMC_NORSRAM_TimingTypeDef FMC_ReadWriteTim;
   FMC_NORSRAM_TimingTypeDef FMC_WriteTim;
	
	 SRAM_Handler.Instance=FMC_NORSRAM_DEVICE;               
   SRAM_Handler.Extended=FMC_NORSRAM_EXTENDED_DEVICE;   
	 SRAM_Handler.Init.NSBank=FMC_NORSRAM_BANK4;    
   SRAM_Handler.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;
   SRAM_Handler.Init.MemoryType=FMC_MEMORY_TYPE_SRAM;   
   SRAM_Handler.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; 
   SRAM_Handler.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE; 
	SRAM_Handler.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;
	SRAM_Handler.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;  
	SRAM_Handler.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;    
	SRAM_Handler.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;         

	SRAM_Handler.Init.ExtendedMode=FMC_EXTENDED_MODE_ENABLE;      
	
	SRAM_Handler.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;
	SRAM_Handler.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           
	SRAM_Handler.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	SRAM_Handler.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
	SRAM_Handler.Init.PageSize = FMC_PAGE_SIZE_NONE;
	
	
	      FMC_ReadWriteTim.AddressSetupTime=1;        //��ַ����ʱ��(ADDSET)Ϊ15��HCLK 1/216M*15=4.6ns*15
        FMC_ReadWriteTim.AddressHoldTime=0;
        FMC_ReadWriteTim.DataSetupTime=15;           //���ݱ���ʱ��(DATAST)Ϊ255��HCLK        =4.6*255
        FMC_ReadWriteTim.BusTurnAroundDuration = 0;
        FMC_ReadWriteTim.CLKDivision=0;
        FMC_ReadWriteTim.DataLatency =0;
        FMC_ReadWriteTim.AccessMode=FMC_ACCESS_MODE_A; //ģʽA
        //FMCдʱ����ƼĴ���
        FMC_WriteTim.AddressSetupTime=2;            //��ַ����ʱ��(ADDSET)Ϊ17��HCLK=82.5ns
        FMC_WriteTim.AddressHoldTime=0;
        FMC_WriteTim.DataSetupTime=15;               //���ݱ���ʱ��(DATAST)Ϊ5.5ns*17��HCLK=82.5ns
        FMC_WriteTim.BusTurnAroundDuration = 0;
        FMC_WriteTim.CLKDivision=16;
        FMC_WriteTim.DataLatency =0;
        FMC_WriteTim.AccessMode=FMC_ACCESS_MODE_A;     //ģʽA
        
        HAL_SRAM_Init(&SRAM_Handler,&FMC_ReadWriteTim,&FMC_WriteTim);    
//	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
//	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
//	
//	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
//	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
//	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
//	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
//	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
//	//FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
//	/* FSMC Write Timing */
//	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 1;//7;   /* Address Setup Time */
//	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
//	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 9;	     /* Data Setup Time */
//	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0x00;
//	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0x00;
//	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0x00;
//	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;//FSMC_AccessMode_A;	/* FSMC Access Mode */
//	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;	  
//	
//	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
//	
//	/* Enable FSMC Bank4_SRAM Bank */
//	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_Configuration(void)
{
	uint32_t i=0x1fffff;
	/* Configure the LCD Control pins --------------------------------------------*/
	LCD_CtrlLinesConfig();
	while(i--);
	/* Configure the FSMC Parallel interface -------------------------------------*/
	LCD_FSMCConfig();
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void delay_ms(uint16_t ms)    
{ 
	HAL_Delay(ms);
//	uint16_t i,j; 
//	for( i = 0; i < ms; i++ )
//	{ 
//		for( j = 0; j < 11410; j++ );
//	}
} 

/*******************************************************************************
* Function Name  : LCD_Initializtion
* Description    : SSD1963 Resets
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
	uint16_t DeviceCode;
	uint16_t i;

void LCD_Initializtion(void)
{
	/*LCD RESET PIN init*/

														
	LCD_Configuration();

	/* Set MN(multipliers) of PLL, VCO = crystal freq * (N+1) */
	/* PLL freq = VCO/M with 250MHz < VCO < 800MHz */
	/* The max PLL freq is around 120MHz. To obtain 120MHz as the PLL freq */

	DeviceCode = 0x0123;
	DeviceCode = LCD_ReadReg(0x0000);		/* Read LCD ID	*/	
	
//   SerialPutChar(DeviceCode>>8);
// 	SerialPutChar(DeviceCode);
	
	printf("DeviceCode %x\r\n",DeviceCode);

	if( DeviceCode == 0x9325 || DeviceCode == 0x9328  || DeviceCode == 0x8325)	
	{
		LCD_Code = ILI9325;
// 		LCD_WriteReg(0x00e7,0x0010);      
// 		LCD_WriteReg(0x0000,0x0001);  	/* start internal osc */
// 		LCD_WriteReg(0x0001,0x0100);     
// 		LCD_WriteReg(0x0002,0x0700); 	/* power on sequence */
// 		LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4)|(0<<3) ); 	/* importance */
// 		LCD_WriteReg(0x0004,0x0000);                                   
// 		LCD_WriteReg(0x0008,0x0207);	           
// 		LCD_WriteReg(0x0009,0x0000);         
// 		LCD_WriteReg(0x000a,0x0000); 	/* display setting */        
// 		LCD_WriteReg(0x000c,0x0001);	/* display setting */        
// 		LCD_WriteReg(0x000d,0x0000); 			        
// 		LCD_WriteReg(0x000f,0x0000);
// 		/* Power On sequence */
// 		LCD_WriteReg(0x0010,0x0000);   
// 		LCD_WriteReg(0x0011,0x0007);
// 		LCD_WriteReg(0x0012,0x0000);                                                                 
// 		LCD_WriteReg(0x0013,0x0000);                 
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0010,0x1590);   
// 		LCD_WriteReg(0x0011,0x0227);
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0012,0x009c);                  
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0013,0x1900);   
// 		LCD_WriteReg(0x0029,0x0023);
// 		
// 		LCD_WriteReg(0x002b,0x000e);
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0020,0x0000);                                                            
// 		LCD_WriteReg(0x0021,0x0000);           
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0030,0x0007); 
// 		LCD_WriteReg(0x0031,0x0707);   
// 		LCD_WriteReg(0x0032,0x0006);
// 		LCD_WriteReg(0x0035,0x0704);
// 		LCD_WriteReg(0x0036,0x1f04); 
// 		LCD_WriteReg(0x0037,0x0004);
// 		LCD_WriteReg(0x0038,0x0000);        
// 		LCD_WriteReg(0x0039,0x0706);     
// 		LCD_WriteReg(0x003c,0x0701);
// 		LCD_WriteReg(0x003d,0x000f);
// 		delay_ms(50);  /* delay 50 ms */		
// 		LCD_WriteReg(0x0050,0x0000);        
// 		LCD_WriteReg(0x0051,0x00ef);   
// 		LCD_WriteReg(0x0052,0x0000);     
// 		LCD_WriteReg(0x0053,0x013f);
// 		LCD_WriteReg(0x0060,0xa700);        
// 		LCD_WriteReg(0x0061,0x0001); 
// 		LCD_WriteReg(0x006a,0x0000);
// 		LCD_WriteReg(0x0080,0x0000);
// 		LCD_WriteReg(0x0081,0x0000);
// 		LCD_WriteReg(0x0082,0x0000);
// 		LCD_WriteReg(0x0083,0x0000);
// 		LCD_WriteReg(0x0084,0x0000);
// 		LCD_WriteReg(0x0085,0x0000);
// 		  
// 		LCD_WriteReg(0x0090,0x0010);     
// 		LCD_WriteReg(0x0092,0x0000);  
// 		LCD_WriteReg(0x0093,0x0003);
// 		LCD_WriteReg(0x0095,0x0110);
// 		LCD_WriteReg(0x0097,0x0000);        
// 		LCD_WriteReg(0x0098,0x0000);  
// 		/* display on sequence */    
// 		LCD_WriteReg(0x0007,0x0133);
// 		
// 		LCD_WriteReg(0x0020,0x0000);
// 		LCD_WriteReg(0x0021,0x0000);


// /* Start Initial Sequence ----------------------------------------------------*/
//   LCD_WriteReg(R229,0x8000); /* Set the internal vcore voltage */
//   LCD_WriteReg(R0,  0x0001); /* Start internal OSC. */
//   LCD_WriteReg(R1,  0x0100); /* set SS and SM bit */
//   LCD_WriteReg(R2,  0x0700); /* set 1 line inversion */
//  // LCD_WriteReg(R3,  0x1030); /* set GRAM write direction and BGR=1. */
//   LCD_WriteReg(0x0003,(1<<12)|(3<<4)|(0<<3) );
//   LCD_WriteReg(R4,  0x0000); /* Resize register */
//   LCD_WriteReg(R8,  0x0202); /* set the back porch and front porch */
//   LCD_WriteReg(R9,  0x0000); /* set non-display area refresh cycle ISC[3:0] */
//   LCD_WriteReg(R10, 0x0000); /* FMARK function */
//   LCD_WriteReg(R12, 0x0000); /* RGB interface setting */
//   LCD_WriteReg(R13, 0x0000); /* Frame marker Position */
//   LCD_WriteReg(R15, 0x0000); /* RGB interface polarity */

// /* Power On sequence ---------------------------------------------------------*/
//   LCD_WriteReg(R16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
//   LCD_WriteReg(R17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
//   LCD_WriteReg(R18, 0x0000); /* VREG1OUT voltage */
//   LCD_WriteReg(R19, 0x0000); /* VDV[4:0] for VCOM amplitude */
//   Delay(100);                 /* Dis-charge capacitor power voltage (200ms) */
//   LCD_WriteReg(R16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
//   LCD_WriteReg(R17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
//   Delay(50);                  /* Delay 50 ms */
//   LCD_WriteReg(R18, 0x0139); /* VREG1OUT voltage */
//   Delay(50);                  /* Delay 50 ms */
//   LCD_WriteReg(R19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
//   LCD_WriteReg(R41, 0x0013); /* VCM[4:0] for VCOMH */
//   Delay(50);                  /* Delay 50 ms */
//   LCD_WriteReg(R32, 0x0000); /* GRAM horizontal Address */
//   LCD_WriteReg(R33, 0x0000); /* GRAM Vertical Address */

// /* Adjust the Gamma Curve ----------------------------------------------------*/
//   LCD_WriteReg(R48, 0x0006);
//   LCD_WriteReg(R49, 0x0101);
//   LCD_WriteReg(R50, 0x0003);
//   LCD_WriteReg(R53, 0x0106);
//   LCD_WriteReg(R54, 0x0b02);
//   LCD_WriteReg(R55, 0x0302);
//   LCD_WriteReg(R56, 0x0707);
//   LCD_WriteReg(R57, 0x0007);
//   LCD_WriteReg(R60, 0x0600);
//   LCD_WriteReg(R61, 0x020b);
//   
// /* Set GRAM area -------------------------------------------------------------*/
//   LCD_WriteReg(R80, 0x0000); /* Horizontal GRAM Start Address */
//   LCD_WriteReg(R81, 0x00EF); /* Horizontal GRAM End Address */
//   LCD_WriteReg(R82, 0x0000); /* Vertical GRAM Start Address */
//   LCD_WriteReg(R83, 0x013F); /* Vertical GRAM End Address */

//   LCD_WriteReg(R96,  0xa700); /* Gate Scan Line */
//   LCD_WriteReg(R97,  0x0001); /* NDL,VLE, REV */
//   LCD_WriteReg(R106, 0x0000); /* set scrolling line */

// /* Partial Display Control ---------------------------------------------------*/
//   LCD_WriteReg(R128, 0x0000);
//   LCD_WriteReg(R129, 0x0000);
//   LCD_WriteReg(R130, 0x0000);
//   LCD_WriteReg(R131, 0x0000);
//   LCD_WriteReg(R132, 0x0000);
//   LCD_WriteReg(R133, 0x0000);

// /* Panel Control -------------------------------------------------------------*/
//   LCD_WriteReg(R144, 0x0010);
//   LCD_WriteReg(R146, 0x0000);
//   LCD_WriteReg(R147, 0x0003);
//   LCD_WriteReg(R149, 0x0110);
//   LCD_WriteReg(R151, 0x0000);
//   LCD_WriteReg(R152, 0x0000);

//   /* Set GRAM write direction and BGR = 1 */
//   /* I/D=01 (Horizontal : increment, Vertical : decrement) */
//   /* AM=1 (address is updated in vertical writing direction) */
//  //LCD_WriteReg(R3, 0x1018);
//   LCD_WriteReg(0x03, 0x1038);
// //LCD_WriteReg(R3,(1<<12)|(0<<5)|(0<<4)|(1<<3) ); 
//   LCD_WriteReg(R7, 0x0173); /* 262K color and display ON */  
// 	



//LCD_WriteReg(0x0001,0x0100); 
// LCD_WriteReg(0x0002,0x0700); 
// LCD_WriteReg(0x0003,0x1030); 
// LCD_WriteReg(0x0004,0x0000); 
// LCD_WriteReg(0x0008,0x0207);  
// LCD_WriteReg(0x0009,0x0000);
// LCD_WriteReg(0x000A,0x0000); 
// LCD_WriteReg(0x000C,0x0000); 
// LCD_WriteReg(0x000D,0x0000);
// LCD_WriteReg(0x000F,0x0000);
////power on sequence VGHVGL
// LCD_WriteReg(0x0010,0x0000);   
// LCD_WriteReg(0x0011,0x0007);  
// LCD_WriteReg(0x0012,0x0000);  
// LCD_WriteReg(0x0013,0x0000); 
////vgh 
// LCD_WriteReg(0x0010,0x1290);   
// LCD_WriteReg(0x0011,0x0227);
// delay_ms(100);
// //vregiout 
// LCD_WriteReg(0x0012,0x001d); //0x001b
// delay_ms(100); 
// //vom amplitude
// LCD_WriteReg(0x0013,0x1500);
// //delayms(100); 
// //vom H
// LCD_WriteReg(0x0029,0x0018); 
// LCD_WriteReg(0x002B,0x000D); 

////gamma
// LCD_WriteReg(0x0030,0x0004);
// LCD_WriteReg(0x0031,0x0307);
// LCD_WriteReg(0x0032,0x0002);// 0006
// LCD_WriteReg(0x0035,0x0206);
// LCD_WriteReg(0x0036,0x0408);
// LCD_WriteReg(0x0037,0x0507); 
// LCD_WriteReg(0x0038,0x0204);//0200
// LCD_WriteReg(0x0039,0x0707); 
// LCD_WriteReg(0x003C,0x0405);// 0504
// LCD_WriteReg(0x003D,0x0F02); 
// //ram
// LCD_WriteReg(0x0050,0x0000); 
// LCD_WriteReg(0x0051,0x00EF);
// LCD_WriteReg(0x0052,0x0000); 
// LCD_WriteReg(0x0053,0x013F);  
// LCD_WriteReg(0x0060,0xA700); 
// LCD_WriteReg(0x0061,0x0001); 
// LCD_WriteReg(0x006A,0x0000); 
// //
// LCD_WriteReg(0x0080,0x0000); 
// LCD_WriteReg(0x0081,0x0000); 
// LCD_WriteReg(0x0082,0x0000); 
// LCD_WriteReg(0x0083,0x0000); 
// LCD_WriteReg(0x0084,0x0000); 
// LCD_WriteReg(0x0085,0x0000); 
// //
// LCD_WriteReg(0x0090,0x0010); 
// LCD_WriteReg(0x0092,0x0600); 
// LCD_WriteReg(0x0093,0x0003); 
// LCD_WriteReg(0x0095,0x0110); 
// LCD_WriteReg(0x0097,0x0000); 
// LCD_WriteReg(0x0098,0x0000);
// LCD_WriteReg(0x0007,0x0133);
	LCD_WriteReg(0x00E5,0x78F0); 
		LCD_WriteReg(0x0001,0x0100); 
		LCD_WriteReg(0x0002,0x0700); 
		LCD_WriteReg(0x0003,0x1030); 
		LCD_WriteReg(0x0004,0x0000); 
		LCD_WriteReg(0x0008,0x0202);  
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000); 
		LCD_WriteReg(0x000C,0x0000); 
		LCD_WriteReg(0x000D,0x0000);
		LCD_WriteReg(0x000F,0x0000);
		//power on sequence VGHVGL
		LCD_WriteReg(0x0010,0x0000);   
		LCD_WriteReg(0x0011,0x0007);  
		LCD_WriteReg(0x0012,0x0000);  
		LCD_WriteReg(0x0013,0x0000); 
		LCD_WriteReg(0x0007,0x0000); 
		//vgh 
		LCD_WriteReg(0x0010,0x1690);   
		LCD_WriteReg(0x0011,0x0227);
		//delayms(100);
		//vregiout 
		LCD_WriteReg(0x0012,0x009D); //0x001b
		//delayms(100); 
		//vom amplitude
		LCD_WriteReg(0x0013,0x1900);
		//delayms(100); 
		//vom H
		LCD_WriteReg(0x0029,0x0025); 
		LCD_WriteReg(0x002B,0x000D); 
		//gamma
		LCD_WriteReg(0x0030,0x0007);
		LCD_WriteReg(0x0031,0x0303);
		LCD_WriteReg(0x0032,0x0003);// 0006
		LCD_WriteReg(0x0035,0x0206);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0406); 
		LCD_WriteReg(0x0038,0x0304);//0200
		LCD_WriteReg(0x0039,0x0007); 
		LCD_WriteReg(0x003C,0x0602);// 0504
		LCD_WriteReg(0x003D,0x0008); 
		//ram
		LCD_WriteReg(0x0050,0x0000); 
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000); 
		LCD_WriteReg(0x0053,0x013F);  
		LCD_WriteReg(0x0060,0xA700); 
		LCD_WriteReg(0x0061,0x0001); 
		LCD_WriteReg(0x006A,0x0000); 
		//
		LCD_WriteReg(0x0080,0x0000); 
		LCD_WriteReg(0x0081,0x0000); 
		LCD_WriteReg(0x0082,0x0000); 
		LCD_WriteReg(0x0083,0x0000); 
		LCD_WriteReg(0x0084,0x0000); 
		LCD_WriteReg(0x0085,0x0000); 
		//
		LCD_WriteReg(0x0090,0x0010); 
		LCD_WriteReg(0x0092,0x0600); 
		
		LCD_WriteReg(0x0007,0x0133);
		LCD_WriteReg(0x00,0x0022);//
	}
	else if( DeviceCode == 0x9331 )
	{
	    LCD_Code = ILI9331;
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100);   /* set SS and SM bit */
		LCD_WriteReg(0x0002, 0x0200);   /* set 1 line inversion */
		LCD_WriteReg(0x0003, 0x1030);   /* set GRAM write direction and BGR=1 */
		LCD_WriteReg(0x0008, 0x0202);   /* set the back porch and front porch */
	    LCD_WriteReg(0x0009, 0x0000);   /* set non-display area refresh cycle ISC[3:0] */
		LCD_WriteReg(0x000A, 0x0000);   /* FMARK function */
		LCD_WriteReg(0x000C, 0x0000);   /* RGB interface setting */
		LCD_WriteReg(0x000D, 0x0000);   /* Frame marker Position */
		LCD_WriteReg(0x000F, 0x0000);   /* RGB interface polarity */
		/* Power On sequence */
		LCD_WriteReg(0x0010, 0x0000);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
		LCD_WriteReg(0x0011, 0x0007);   /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x0012, 0x0000);   /* VREG1OUT voltage	*/
		LCD_WriteReg(0x0013, 0x0000);   /* VDV[4:0] for VCOM amplitude */
	    delay_ms(200);  /* delay 200 ms */		
		LCD_WriteReg(0x0010, 0x1690);   /* SAP, BT[3:0], AP, DSTB, SLP, STB	*/
		LCD_WriteReg(0x0011, 0x0227);   /* DC1[2:0], DC0[2:0], VC[2:0] */
	    delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0012, 0x000C);   /* Internal reference voltage= Vci	*/
	    delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0013, 0x0800);   /* Set VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x0029, 0x0011);   /* Set VCM[5:0] for VCOMH */
		LCD_WriteReg(0x002B, 0x000B);   /* Set Frame Rate */
	    delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0020, 0x0000);   /* GRAM horizontal Address */
		LCD_WriteReg(0x0021, 0x0000);   /* GRAM Vertical Address */
		/* Adjust the Gamma Curve */
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
		/* Set GRAM area */
		LCD_WriteReg(0x0050, 0x0000);   /* Horizontal GRAM Start Address */
		LCD_WriteReg(0x0051, 0x00EF);   /* Horizontal GRAM End Address */
		LCD_WriteReg(0x0052, 0x0000);   /* Vertical GRAM Start Address */
		LCD_WriteReg(0x0053, 0x013F);   /* Vertical GRAM Start Address */
		LCD_WriteReg(0x0060, 0x2700);   /* Gate Scan Line */
		LCD_WriteReg(0x0061, 0x0001);   /*  NDL,VLE, REV */
		LCD_WriteReg(0x006A, 0x0000);   /* set scrolling line */
		/* Partial Display Control */
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		/* Panel Control */
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);		
	    delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0007,0x0061);
	    delay_ms(50);  /* delay 50 ms */		
		LCD_WriteReg(0x0007,0x0133);    /* 262K color and display ON */
		
		
 	}
	else if( DeviceCode == 0x9320 || DeviceCode == 0x9300 )
	{
	    LCD_Code = ILI9320;
	    LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	/* Driver Output Contral */
		LCD_WriteReg(0x02,0x0700);	/* LCD Driver Waveform Contral */
		LCD_WriteReg(0x03,0x1018);	/* Entry Mode Set */
		
		LCD_WriteReg(0x04,0x0000);	/* Scalling Contral */
	    LCD_WriteReg(0x08,0x0202);	/* Display Contral */
		LCD_WriteReg(0x09,0x0000);	/* Display Contral 3.(0x0000) */
		LCD_WriteReg(0x0a,0x0000);	/* Frame Cycle Contal.(0x0000) */
	    LCD_WriteReg(0x0c,(1<<0));	/* Extern Display Interface Contral */
		LCD_WriteReg(0x0d,0x0000);	/* Frame Maker Position */
		LCD_WriteReg(0x0f,0x0000);	/* Extern Display Interface Contral 2. */
		
	    delay_ms(100);  /* delay 100 ms */		
		LCD_WriteReg(0x07,0x0101);	/* Display Contral */
	    delay_ms(100);  /* delay 100 ms */		
	
		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	/* Power Control 1.(0x16b0)	*/
		LCD_WriteReg(0x11,0x0007);								/* Power Control 2 */
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));				/* Power Control 3.(0x0138)	*/
		LCD_WriteReg(0x13,0x0b00);								/* Power Control 4 */
		LCD_WriteReg(0x29,0x0000);								/* Power Control 7 */
		
		LCD_WriteReg(0x2b,(1<<14)|(1<<4));
			
		LCD_WriteReg(0x50,0);       /* Set X Start */
		LCD_WriteReg(0x51,239);	    /* Set X End */
		LCD_WriteReg(0x52,0);	    /* Set Y Start */
		LCD_WriteReg(0x53,319);	    /* Set Y End */
		
		LCD_WriteReg(0x60,0x2700);	/* Driver Output Control */
		LCD_WriteReg(0x61,0x0001);	/* Driver Output Control */
		LCD_WriteReg(0x6a,0x0000);	/* Vertical Srcoll Control */
		
		LCD_WriteReg(0x80,0x0000);	/* Display Position? Partial Display 1 */
		LCD_WriteReg(0x81,0x0000);	/* RAM Address Start? Partial Display 1 */
		LCD_WriteReg(0x82,0x0000);	/* RAM Address End-Partial Display 1 */
		LCD_WriteReg(0x83,0x0000);	/* Displsy Position? Partial Display 2 */
		LCD_WriteReg(0x84,0x0000);	/* RAM Address Start? Partial Display 2 */
		LCD_WriteReg(0x85,0x0000);	/* RAM Address End? Partial Display 2 */
		
	    LCD_WriteReg(0x90,(0<<7)|(16<<0));	/* Frame Cycle Contral.(0x0013)	*/
		LCD_WriteReg(0x92,0x0000);	/* Panel Interface Contral 2.(0x0000) */
		LCD_WriteReg(0x93,0x0001);	/* Panel Interface Contral 3. */
	    LCD_WriteReg(0x95,0x0110);	/* Frame Cycle Contral.(0x0110)	*/
		LCD_WriteReg(0x97,(0<<8));	
		LCD_WriteReg(0x98,0x0000);	/* Frame Cycle Contral */
	
	    LCD_WriteReg(0x07,0x0173);
		
	}
	else if( DeviceCode == 0x9919 )
	{
	    LCD_Code = SSD2119;
	    /* POWER ON &RESET DISPLAY OFF */
		LCD_WriteReg(0x28,0x0006);
		LCD_WriteReg(0x00,0x0001);		
		LCD_WriteReg(0x10,0x0000);		
		LCD_WriteReg(0x01,0x72ef);
		LCD_WriteReg(0x02,0x0600);
		LCD_WriteReg(0x03,0x6a38);	
		LCD_WriteReg(0x11,0x6874);
		LCD_WriteReg(0x0f,0x0000);    /* RAM WRITE DATA MASK */
		LCD_WriteReg(0x0b,0x5308);    /* RAM WRITE DATA MASK */
		LCD_WriteReg(0x0c,0x0003);
		LCD_WriteReg(0x0d,0x000a);
		LCD_WriteReg(0x0e,0x2e00);  
		LCD_WriteReg(0x1e,0x00be);
		LCD_WriteReg(0x25,0x8000);
		LCD_WriteReg(0x26,0x7800);
		LCD_WriteReg(0x27,0x0078);
		LCD_WriteReg(0x4e,0x0000);
		LCD_WriteReg(0x4f,0x0000);
		LCD_WriteReg(0x12,0x08d9);
		/* Adjust the Gamma Curve */
		LCD_WriteReg(0x30,0x0000);
		LCD_WriteReg(0x31,0x0104);	 
		LCD_WriteReg(0x32,0x0100);	
	    LCD_WriteReg(0x33,0x0305);	
	    LCD_WriteReg(0x34,0x0505);	 
		LCD_WriteReg(0x35,0x0305);	
	    LCD_WriteReg(0x36,0x0707);	
	    LCD_WriteReg(0x37,0x0300);	
		LCD_WriteReg(0x3a,0x1200);	
		LCD_WriteReg(0x3b,0x0800);		 
	    LCD_WriteReg(0x07,0x0033);
	}
	else if( DeviceCode == 0x1505 || DeviceCode == 0x0505 )
	{
	    LCD_Code = R61505U;
	   /* second release on 3/5  ,luminance is acceptable,water wave appear during camera preview */
	    LCD_WriteReg(0x0007,0x0000);
	    delay_ms(50);  /* delay 50 ms */		
	    LCD_WriteReg(0x0012,0x011C);    /* why need to set several times?	*/
	    LCD_WriteReg(0x00A4,0x0001);    /* NVM */
	    LCD_WriteReg(0x0008,0x000F);
	    LCD_WriteReg(0x000A,0x0008);
	    LCD_WriteReg(0x000D,0x0008);    
	    /* GAMMA CONTROL */
	    LCD_WriteReg(0x0030,0x0707);
	    LCD_WriteReg(0x0031,0x0007); 
	    LCD_WriteReg(0x0032,0x0603); 
	    LCD_WriteReg(0x0033,0x0700); 
	    LCD_WriteReg(0x0034,0x0202); 
	    LCD_WriteReg(0x0035,0x0002); 
	    LCD_WriteReg(0x0036,0x1F0F);
	    LCD_WriteReg(0x0037,0x0707); 
	    LCD_WriteReg(0x0038,0x0000); 
	    LCD_WriteReg(0x0039,0x0000); 
	    LCD_WriteReg(0x003A,0x0707); 
	    LCD_WriteReg(0x003B,0x0000); 
	    LCD_WriteReg(0x003C,0x0007); 
	    LCD_WriteReg(0x003D,0x0000); 
	    delay_ms(50);  /* delay 50 ms */		
	    LCD_WriteReg(0x0007,0x0001);
	    LCD_WriteReg(0x0017,0x0001);    /* Power supply startup enable */
	    delay_ms(50);  /* delay 50 ms */		
	    /* power control */
	    LCD_WriteReg(0x0010,0x17A0); 
	    LCD_WriteReg(0x0011,0x0217);    /* reference voltage VC[2:0]   Vciout = 1.00*Vcivl */
	    LCD_WriteReg(0x0012,0x011E);    /* Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?	*/
	    LCD_WriteReg(0x0013,0x0F00);    /* VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl */
	    LCD_WriteReg(0x002A,0x0000);  
	    LCD_WriteReg(0x0029,0x000A);    /* Vcomh = VCM1[4:0]*Vreg1out    gate source voltage?? */
	    LCD_WriteReg(0x0012,0x013E);    /* power supply on */
	    /* Coordinates Control */
	    LCD_WriteReg(0x0050,0x0000);
	    LCD_WriteReg(0x0051,0x00EF); 
	    LCD_WriteReg(0x0052,0x0000); 
	    LCD_WriteReg(0x0053,0x013F); 
	    /* Pannel Image Control */
	    LCD_WriteReg(0x0060,0x2700); 
	    LCD_WriteReg(0x0061,0x0001); 
	    LCD_WriteReg(0x006A,0x0000); 
	    LCD_WriteReg(0x0080,0x0000); 
	    /* Partial Image Control */
	    LCD_WriteReg(0x0081,0x0000); 
	    LCD_WriteReg(0x0082,0x0000); 
	    LCD_WriteReg(0x0083,0x0000); 
	    LCD_WriteReg(0x0084,0x0000); 
	    LCD_WriteReg(0x0085,0x0000); 
	    /* Panel Interface Control */
	    LCD_WriteReg(0x0090,0x0013);      /* frenqucy */	
	    LCD_WriteReg(0x0092,0x0300); 
	    LCD_WriteReg(0x0093,0x0005); 
	    LCD_WriteReg(0x0095,0x0000); 
	    LCD_WriteReg(0x0097,0x0000); 
	    LCD_WriteReg(0x0098,0x0000); 
	  
	    LCD_WriteReg(0x0001,0x0100); 
	    LCD_WriteReg(0x0002,0x0700); 
	    LCD_WriteReg(0x0003,0x1030); 
	    LCD_WriteReg(0x0004,0x0000); 
	    LCD_WriteReg(0x000C,0x0000); 
	    LCD_WriteReg(0x000F,0x0000); 
	    LCD_WriteReg(0x0020,0x0000); 
	    LCD_WriteReg(0x0021,0x0000); 
	    LCD_WriteReg(0x0007,0x0021); 
	    delay_ms(200);  /* delay 200 ms */		
	    LCD_WriteReg(0x0007,0x0061); 
	    delay_ms(200);  /* delay 200 ms */		
	    LCD_WriteReg(0x0007,0x0173); 
	}							 
	else if( DeviceCode == 0x8989 )
	{
	    LCD_Code = SSD1289;
	    LCD_WriteReg(0x0000,0x0001);    delay_ms(50);   /* Enable LCD Oscillator */
	    LCD_WriteReg(0x0003,0xA8A4);    delay_ms(50);   
	    LCD_WriteReg(0x000C,0x0000);    delay_ms(50);   
	    LCD_WriteReg(0x000D,0x080C);    delay_ms(50);   
	    LCD_WriteReg(0x000E,0x2B00);    delay_ms(50);   
	    LCD_WriteReg(0x001E,0x00B0);    delay_ms(50);   
	    LCD_WriteReg(0x0001,0x2B3F);    delay_ms(50);   /* 320*240 0x2B3F */
	    LCD_WriteReg(0x0002,0x0600);    delay_ms(50);
	    LCD_WriteReg(0x0010,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0011,0x6070);    delay_ms(50);
	    LCD_WriteReg(0x0005,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0006,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0016,0xEF1C);    delay_ms(50);
	    LCD_WriteReg(0x0017,0x0003);    delay_ms(50);
	    LCD_WriteReg(0x0007,0x0133);    delay_ms(50);         
	    LCD_WriteReg(0x000B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x000F,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0041,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0042,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0048,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0049,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x004A,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x004B,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0044,0xEF00);    delay_ms(50);
	    LCD_WriteReg(0x0045,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0046,0x013F);    delay_ms(50);
	    LCD_WriteReg(0x0030,0x0707);    delay_ms(50);
	    LCD_WriteReg(0x0031,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0032,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0033,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x0034,0x0507);    delay_ms(50);
	    LCD_WriteReg(0x0035,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0036,0x0204);    delay_ms(50);
	    LCD_WriteReg(0x0037,0x0502);    delay_ms(50);
	    LCD_WriteReg(0x003A,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x003B,0x0302);    delay_ms(50);
	    LCD_WriteReg(0x0023,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0024,0x0000);    delay_ms(50);
	    LCD_WriteReg(0x0025,0x8000);    delay_ms(50);
	    LCD_WriteReg(0x004f,0);
	    LCD_WriteReg(0x004e,0);
	}
	else if( DeviceCode == 0x8999 )
	{
		LCD_Code = SSD1298;		
		LCD_WriteReg(0x0028,0x0006);
		LCD_WriteReg(0x0000,0x0001);
		LCD_WriteReg(0x0003,0xaea4);    /* power control 1---line frequency and VHG,VGL voltage */
		LCD_WriteReg(0x000c,0x0004);    /* power control 2---VCIX2 output voltage */
		LCD_WriteReg(0x000d,0x000c);    /* power control 3---Vlcd63 voltage */
		LCD_WriteReg(0x000e,0x2800);    /* power control 4---VCOMA voltage VCOML=VCOMH*0.9475-VCOMA */
		LCD_WriteReg(0x001e,0x00b5);    /* POWER CONTROL 5---VCOMH voltage */   
		LCD_WriteReg(0x0001,0x3b3f);     
		LCD_WriteReg(0x0002,0x0600);
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x6830);
		LCD_WriteReg(0x0005,0x0000);
		LCD_WriteReg(0x0006,0x0000);
		LCD_WriteReg(0x0016,0xef1c);  
		LCD_WriteReg(0x0007,0x0033);    /* Display control 1 */
		/* when GON=1 and DTE=0,all gate outputs become VGL */
		/* when GON=1 and DTE=0,all gate outputs become VGH */
		/* non-selected gate wires become VGL */     
		LCD_WriteReg(0x000b,0x0000);
		LCD_WriteReg(0x000f,0x0000);
		LCD_WriteReg(0x0041,0x0000);
		LCD_WriteReg(0x0042,0x0000);
		LCD_WriteReg(0x0048,0x0000);
		LCD_WriteReg(0x0049,0x013f);
		LCD_WriteReg(0x004a,0x0000);
		LCD_WriteReg(0x004b,0x0000); 
		LCD_WriteReg(0x0044,0xef00);	/* Horizontal RAM start and end address */
		LCD_WriteReg(0x0045,0x0000);	/* Vretical RAM start address */
		LCD_WriteReg(0x0046,0x013f);	/* Vretical RAM end address */ 
		LCD_WriteReg(0x004e,0x0000);	/* set GDDRAM x address counter */
		LCD_WriteReg(0x004f,0x0000);    /* set GDDRAM y address counter */   
		/* y control */
		LCD_WriteReg(0x0030,0x0707);
		LCD_WriteReg(0x0031,0x0202);
		LCD_WriteReg(0x0032,0x0204);
		LCD_WriteReg(0x0033,0x0502);
		LCD_WriteReg(0x0034,0x0507);
		LCD_WriteReg(0x0035,0x0204);
		LCD_WriteReg(0x0036,0x0204);
		LCD_WriteReg(0x0037,0x0502);
		LCD_WriteReg(0x003a,0x0302);
		LCD_WriteReg(0x003b,0x0302); 
		LCD_WriteReg(0x0023,0x0000);
		LCD_WriteReg(0x0024,0x0000);
		LCD_WriteReg(0x0025,0x8000);
		LCD_WriteReg(0x0026,0x7000);
		LCD_WriteReg(0x0020,0xb0eb);
		LCD_WriteReg(0x0027,0x007c);
	}
	else if( DeviceCode == 0x5408 )
	{
		LCD_Code = SPFD5408B;
		
		LCD_WriteReg(0x0001,0x0100);	  /* Driver Output Contral Register */ 
		LCD_WriteReg(0x0002,0x0700);      /* LCD Driving Waveform Contral */
		LCD_WriteReg(0x0003,0x1030);
		
		LCD_WriteReg(0x0004,0x0000);	  /* Scalling Control register */
		LCD_WriteReg(0x0008,0x0207);	  /* Display Control 2 */
		LCD_WriteReg(0x0009,0x0000);	  /* Display Control 3 */
		LCD_WriteReg(0x000A,0x0000);	  /* Frame Cycle Control */
		LCD_WriteReg(0x000C,0x0000);	  /* External Display Interface Control 1 */
		LCD_WriteReg(0x000D,0x0000);      /* Frame Maker Position */
		LCD_WriteReg(0x000F,0x0000);	  /* External Display Interface Control 2 */
		delay_ms(50);
		LCD_WriteReg(0x0007,0x0101);	  /* Display Control */
		delay_ms(50);
		LCD_WriteReg(0x0010,0x16B0);      /* Power Control 1 */
		LCD_WriteReg(0x0011,0x0001);      /* Power Control 2 */
		LCD_WriteReg(0x0017,0x0001);      /* Power Control 3 */
		LCD_WriteReg(0x0012,0x0138);      /* Power Control 4 */
		LCD_WriteReg(0x0013,0x0800);      /* Power Control 5 */
		LCD_WriteReg(0x0029,0x0009);	  /* NVM read data 2 */
		LCD_WriteReg(0x002a,0x0009);	  /* NVM read data 3 */
		LCD_WriteReg(0x00a4,0x0000);  
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00EF);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013F);
		   
		LCD_WriteReg(0x0060,0x2700);	  /* Driver Output Control */

		LCD_WriteReg(0x0061,0x0003);	  /* Driver Output Control */
		LCD_WriteReg(0x006A,0x0000);	  /* Vertical Scroll Control */
		
		LCD_WriteReg(0x0080,0x0000);	  /* Display Position �C Partial Display 1 */
		LCD_WriteReg(0x0081,0x0000);	  /* RAM Address Start �C Partial Display 1 */
		LCD_WriteReg(0x0082,0x0000);	  /* RAM address End - Partial Display 1 */
		LCD_WriteReg(0x0083,0x0000);	  /* Display Position �C Partial Display 2 */
		LCD_WriteReg(0x0084,0x0000);	  /* RAM Address Start �C Partial Display 2 */
		LCD_WriteReg(0x0085,0x0000);	  /* RAM address End �C Partail Display2 */
		LCD_WriteReg(0x0090,0x0013);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0092,0x0000); 	  /* Panel Interface Control 2 */
		LCD_WriteReg(0x0093,0x0003);	  /* Panel Interface control 3 */
		LCD_WriteReg(0x0095,0x0110);	  /* Frame Cycle Control */
		LCD_WriteReg(0x0007,0x0173);
	}
	else if( DeviceCode == 0x4531 )
	{	
		LCD_Code = LGDP4531;
		/* Setup display */
		LCD_WriteReg(0x00,0x0001);
		LCD_WriteReg(0x10,0x0628);
		LCD_WriteReg(0x12,0x0006);
		LCD_WriteReg(0x13,0x0A32);
		LCD_WriteReg(0x11,0x0040);
		LCD_WriteReg(0x15,0x0050);
		LCD_WriteReg(0x12,0x0016);
		delay_ms(50);
		LCD_WriteReg(0x10,0x5660);
		delay_ms(50);
		LCD_WriteReg(0x13,0x2A4E);
		LCD_WriteReg(0x01,0x0100);
		LCD_WriteReg(0x02,0x0300);	
		LCD_WriteReg(0x03,0x1030);		
		LCD_WriteReg(0x08,0x0202);
		LCD_WriteReg(0x0A,0x0000);
		LCD_WriteReg(0x30,0x0000);
		LCD_WriteReg(0x31,0x0402);
		LCD_WriteReg(0x32,0x0106);
		LCD_WriteReg(0x33,0x0700);
		LCD_WriteReg(0x34,0x0104);
		LCD_WriteReg(0x35,0x0301);
		LCD_WriteReg(0x36,0x0707);
		LCD_WriteReg(0x37,0x0305);
		LCD_WriteReg(0x38,0x0208);
		LCD_WriteReg(0x39,0x0F0B);
		delay_ms(50);
		LCD_WriteReg(0x41,0x0002);
		LCD_WriteReg(0x60,0x2700);
		LCD_WriteReg(0x61,0x0001);
		LCD_WriteReg(0x90,0x0119);
		LCD_WriteReg(0x92,0x010A);
		LCD_WriteReg(0x93,0x0004);
		LCD_WriteReg(0xA0,0x0100);
		delay_ms(50);
		LCD_WriteReg(0x07,0x0133);
		delay_ms(50);
		LCD_WriteReg(0xA0,0x0000);
	}
	else if( DeviceCode == 0x4535 )
	{	
		LCD_Code = LGDP4535;	
		LCD_WriteReg(0x15, 0x0030); 	 /* Set the internal vcore voltage */                                              
		LCD_WriteReg(0x9A, 0x0010); 	 /* Start internal OSC */
		LCD_WriteReg(0x11, 0x0020);	     /* set SS and SM bit */
		LCD_WriteReg(0x10, 0x3428);	     /* set 1 line inversion */
		LCD_WriteReg(0x12, 0x0002);	     /* set GRAM write direction and BGR=1 */ 
		LCD_WriteReg(0x13, 0x1038);	     /* Resize register */
		delay_ms(40); 
		LCD_WriteReg(0x12, 0x0012);	     /* set the back porch and front porch */
		delay_ms(40); 
		LCD_WriteReg(0x10, 0x3420);	     /* set non-display area refresh cycle ISC[3:0] */
		LCD_WriteReg(0x13, 0x3045);	     /* FMARK function */
		delay_ms(70); 
		LCD_WriteReg(0x30, 0x0000);      /* RGB interface setting */
		LCD_WriteReg(0x31, 0x0402);	     /* Frame marker Position */
		LCD_WriteReg(0x32, 0x0307);      /* RGB interface polarity */
		LCD_WriteReg(0x33, 0x0304);      /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x34, 0x0004);      /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x35, 0x0401);      /* VREG1OUT voltage */
		LCD_WriteReg(0x36, 0x0707);      /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x37, 0x0305);      /* SAP, BT[3:0], AP, DSTB, SLP, STB */
		LCD_WriteReg(0x38, 0x0610);      /* DC1[2:0], DC0[2:0], VC[2:0] */
		LCD_WriteReg(0x39, 0x0610);      /* VREG1OUT voltage */
		LCD_WriteReg(0x01, 0x0100);      /* VDV[4:0] for VCOM amplitude */
		LCD_WriteReg(0x02, 0x0300);      /* VCM[4:0] for VCOMH */
		LCD_WriteReg(0x03, 0x1030);      /* GRAM horizontal Address */
		LCD_WriteReg(0x08, 0x0808);      /* GRAM Vertical Address */
		LCD_WriteReg(0x0A, 0x0008);		
		LCD_WriteReg(0x60, 0x2700);	     /* Gate Scan Line */
		LCD_WriteReg(0x61, 0x0001);	     /* NDL,VLE, REV */
		LCD_WriteReg(0x90, 0x013E);
		LCD_WriteReg(0x92, 0x0100);
		LCD_WriteReg(0x93, 0x0100);
		LCD_WriteReg(0xA0, 0x3000);
		LCD_WriteReg(0xA3, 0x0010);
		LCD_WriteReg(0x07, 0x0001);
		LCD_WriteReg(0x07, 0x0021);
		LCD_WriteReg(0x07, 0x0023);
		LCD_WriteReg(0x07, 0x0033);
		LCD_WriteReg(0x07, 0x0133);
	} 		 		
	else if( DeviceCode == 0x0047 )
	{
		LCD_Code = HX8347D;
		/* Start Initial Sequence */
		LCD_WriteReg(0xEA,0x00);                          
		LCD_WriteReg(0xEB,0x20);                                                     
		LCD_WriteReg(0xEC,0x0C);                                                   
		LCD_WriteReg(0xED,0xC4);                                                    
		LCD_WriteReg(0xE8,0x40);                                                     
		LCD_WriteReg(0xE9,0x38);                                                    
		LCD_WriteReg(0xF1,0x01);                                                    
		LCD_WriteReg(0xF2,0x10);                                                    
		LCD_WriteReg(0x27,0xA3);                                                    
		/* GAMMA SETTING */
		LCD_WriteReg(0x40,0x01);                           
		LCD_WriteReg(0x41,0x00);                                                   
		LCD_WriteReg(0x42,0x00);                                                   
		LCD_WriteReg(0x43,0x10);                                                    
		LCD_WriteReg(0x44,0x0E);                                                   
		LCD_WriteReg(0x45,0x24);                                                  
		LCD_WriteReg(0x46,0x04);                                                  
		LCD_WriteReg(0x47,0x50);                                                   
		LCD_WriteReg(0x48,0x02);                                                    
		LCD_WriteReg(0x49,0x13);                                                  
		LCD_WriteReg(0x4A,0x19);                                                  
		LCD_WriteReg(0x4B,0x19);                                                 
		LCD_WriteReg(0x4C,0x16);                                                 
		LCD_WriteReg(0x50,0x1B);                                                   
		LCD_WriteReg(0x51,0x31);                                                    
		LCD_WriteReg(0x52,0x2F);                                                     
		LCD_WriteReg(0x53,0x3F);                                                    
		LCD_WriteReg(0x54,0x3F);                                                     
		LCD_WriteReg(0x55,0x3E);                                                     
		LCD_WriteReg(0x56,0x2F);                                                   
		LCD_WriteReg(0x57,0x7B);                                                     
		LCD_WriteReg(0x58,0x09);                                                  
		LCD_WriteReg(0x59,0x06);                                                 
		LCD_WriteReg(0x5A,0x06);                                                   
		LCD_WriteReg(0x5B,0x0C);                                                   
		LCD_WriteReg(0x5C,0x1D);                                                   
		LCD_WriteReg(0x5D,0xCC);                                                   
		/* Power Voltage Setting */
		LCD_WriteReg(0x1B,0x18);                                                    
		LCD_WriteReg(0x1A,0x01);                                                    
		LCD_WriteReg(0x24,0x15);                                                    
		LCD_WriteReg(0x25,0x50);                                                    
		LCD_WriteReg(0x23,0x8B);                                                   
		LCD_WriteReg(0x18,0x36);                           
		LCD_WriteReg(0x19,0x01);                                                    
		LCD_WriteReg(0x01,0x00);                                                   
		LCD_WriteReg(0x1F,0x88);                                                    
		delay_ms(50);
		LCD_WriteReg(0x1F,0x80);                                                  
		delay_ms(50);
		LCD_WriteReg(0x1F,0x90);                                                   
		delay_ms(50);
		LCD_WriteReg(0x1F,0xD0);                                                   
		delay_ms(50);
		LCD_WriteReg(0x17,0x05);                                                    
		LCD_WriteReg(0x36,0x00);                                                    
		LCD_WriteReg(0x28,0x38);                                                 
		delay_ms(50);
		LCD_WriteReg(0x28,0x3C);                                                
	}
	else if( DeviceCode == 0x7783 )
	{
		LCD_Code = ST7781;
		/* Start Initial Sequence */
		LCD_WriteReg(0x00FF,0x0001);
		LCD_WriteReg(0x00F3,0x0008);
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);  
		LCD_WriteReg(0x0008,0x0302);
		LCD_WriteReg(0x0008,0x0207);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000A,0x0000);
		LCD_WriteReg(0x0010,0x0000);  
		LCD_WriteReg(0x0011,0x0005);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		delay_ms(50);
		LCD_WriteReg(0x0010,0x12B0);
		delay_ms(50);
		LCD_WriteReg(0x0011,0x0007);
		delay_ms(50);
		LCD_WriteReg(0x0012,0x008B);
		delay_ms(50);	
		LCD_WriteReg(0x0013,0x1700);
		delay_ms(50);	
		LCD_WriteReg(0x0029,0x0022);		
		LCD_WriteReg(0x0030,0x0000);
		LCD_WriteReg(0x0031,0x0707);
		LCD_WriteReg(0x0032,0x0505);
		LCD_WriteReg(0x0035,0x0107);
		LCD_WriteReg(0x0036,0x0008);
		LCD_WriteReg(0x0037,0x0000);
		LCD_WriteReg(0x0038,0x0202);
		LCD_WriteReg(0x0039,0x0106);
		LCD_WriteReg(0x003C,0x0202);
		LCD_WriteReg(0x003D,0x0408);
		delay_ms(50);				
		LCD_WriteReg(0x0050,0x0000);		
		LCD_WriteReg(0x0051,0x00EF);		
		LCD_WriteReg(0x0052,0x0000);		
		LCD_WriteReg(0x0053,0x013F);		
		LCD_WriteReg(0x0060,0xA700);		
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x0090,0x0033);				
		LCD_WriteReg(0x002B,0x000B);		
		LCD_WriteReg(0x0007,0x0133);
	}
	else	/* special ID */
	{
		DeviceCode = 0x0123;
		DeviceCode = LCD_ReadReg(0xD3);
		LCD_ReadData();
		DeviceCode=LCD_ReadData();
		DeviceCode=DeviceCode<<8 | LCD_ReadData();
		printf("DeviceCode %x\r\n",DeviceCode);
		
		if(DeviceCode==0x9341){ //Howard 
		
	  LCD_Code =ILI9341;
		LCD_WR_REG(0xCF);  
		LCD_WR_DATA(0x00);

		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0xB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_ms(120);
		LCD_WR_REG(0x29); //display on	
		LCD_Display_Dir(0);		 	//Ĭ��Ϊ����
		}
		if( DeviceCode == 0x0047 )
		{
	        LCD_Code = HX8347A;
	        LCD_WriteReg(0x0042,0x0008);   
	        /* Gamma setting */  
		    LCD_WriteReg(0x0046,0x00B4); 
		    LCD_WriteReg(0x0047,0x0043); 
		    LCD_WriteReg(0x0048,0x0013); 
		    LCD_WriteReg(0x0049,0x0047); 
		    LCD_WriteReg(0x004A,0x0014); 
		    LCD_WriteReg(0x004B,0x0036); 
		    LCD_WriteReg(0x004C,0x0003); 
		    LCD_WriteReg(0x004D,0x0046); 
		    LCD_WriteReg(0x004E,0x0005);  
		    LCD_WriteReg(0x004F,0x0010);  
		    LCD_WriteReg(0x0050,0x0008);  
		    LCD_WriteReg(0x0051,0x000a);  
		    /* Window Setting */
		    LCD_WriteReg(0x0002,0x0000); 
		    LCD_WriteReg(0x0003,0x0000); 
		    LCD_WriteReg(0x0004,0x0000); 
		    LCD_WriteReg(0x0005,0x00EF); 
		    LCD_WriteReg(0x0006,0x0000); 
		    LCD_WriteReg(0x0007,0x0000); 
		    LCD_WriteReg(0x0008,0x0001); 
		    LCD_WriteReg(0x0009,0x003F); 
		    delay_ms(10); 
		    LCD_WriteReg(0x0001,0x0006); 
		    LCD_WriteReg(0x0016,0x00C8);   
		    LCD_WriteReg(0x0023,0x0095); 
		    LCD_WriteReg(0x0024,0x0095); 
		    LCD_WriteReg(0x0025,0x00FF); 
		    LCD_WriteReg(0x0027,0x0002); 
		    LCD_WriteReg(0x0028,0x0002); 
		    LCD_WriteReg(0x0029,0x0002); 
		    LCD_WriteReg(0x002A,0x0002); 
		    LCD_WriteReg(0x002C,0x0002); 
		    LCD_WriteReg(0x002D,0x0002); 
		    LCD_WriteReg(0x003A,0x0001);  
		    LCD_WriteReg(0x003B,0x0001);  
		    LCD_WriteReg(0x003C,0x00F0);    
		    LCD_WriteReg(0x003D,0x0000); 
		    delay_ms(20); 
		    LCD_WriteReg(0x0035,0x0038); 
		    LCD_WriteReg(0x0036,0x0078); 
		    LCD_WriteReg(0x003E,0x0038); 
		    LCD_WriteReg(0x0040,0x000F); 
		    LCD_WriteReg(0x0041,0x00F0);  
		    LCD_WriteReg(0x0038,0x0000); 
		    /* Power Setting */ 
		    LCD_WriteReg(0x0019,0x0049);  
		    LCD_WriteReg(0x0093,0x000A); 
		    delay_ms(10); 
		    LCD_WriteReg(0x0020,0x0020);   
		    LCD_WriteReg(0x001D,0x0003);   
		    LCD_WriteReg(0x001E,0x0000);  
		    LCD_WriteReg(0x001F,0x0009);   
		    LCD_WriteReg(0x0044,0x0053);  
		    LCD_WriteReg(0x0045,0x0010);   
		    delay_ms(10);  
		    LCD_WriteReg(0x001C,0x0004);  
		    delay_ms(20); 
		    LCD_WriteReg(0x0043,0x0080);    
		    delay_ms(5); 
		    LCD_WriteReg(0x001B,0x000a);    
		    delay_ms(40);  
		    LCD_WriteReg(0x001B,0x0012);    
		    delay_ms(40);   
		    /* Display On Setting */ 
		    LCD_WriteReg(0x0090,0x007F); 
		    LCD_WriteReg(0x0026,0x0004); 
		    delay_ms(40);  
		    LCD_WriteReg(0x0026,0x0024); 
		    LCD_WriteReg(0x0026,0x002C); 
		    delay_ms(40);   
		    LCD_WriteReg(0x0070,0x0008); 
		    LCD_WriteReg(0x0026,0x003C);  
		    LCD_WriteReg(0x0057,0x0002); 
		    LCD_WriteReg(0x0055,0x0000); 
		    LCD_WriteReg(0x0057,0x0000); 
		}	 
	}  						
    delay_ms(50);   /* delay 50 ms */		
}

/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : - Color: Screen Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index=0;
	
	if( LCD_Code == HX8347D || LCD_Code == HX8347A )
	{
		LCD_WriteReg(0x02,0x00);                                                  
		LCD_WriteReg(0x03,0x00);  
		                
		LCD_WriteReg(0x04,0x00);                           
		LCD_WriteReg(0x05,0xEF);  
		                 
		LCD_WriteReg(0x06,0x00);                           
		LCD_WriteReg(0x07,0x00);    
		               
		LCD_WriteReg(0x08,0x01);                           
		LCD_WriteReg(0x09,0x3F);     
	}else if(LCD_Code==ILI9341)
	{
		LCD_WR_REG(0x2a); 
		LCD_WR_DATA(0>>8); 
		LCD_WR_DATA(0&0XFF);	 
		LCD_WR_REG(0x2b); 
		LCD_WR_DATA(0>>8); 
		LCD_WR_DATA(0&0XFF);
	}
	else
	{	
		//LCD_SetCursor(0,0); 
		

//		printf("ss\r\n");
//		LCD_WriteReg(0x50, 0); //水平GRAM起始位置 
//		LCD_WriteReg(0x51, 240); //水平GRAM终点位置 
//		LCD_WriteReg(0x52, 0); //水平GRAM起始位置
//		LCD_WriteReg(0x53, 319); //垂直GRAM终点位置
//		LCD_WriteReg(0x20, 0x0000);
//		LCD_WriteReg(0x21, 0x0000);
	LCD_WriteReg(0x21, 0);
		LCD_WriteReg(0x22, 0);
	}	
if(LCD_Code==ILI9341){
	LCD_WriteIndex(0x002C);
}else{
	LCD_WriteIndex(0x0022);
}
	for( index = 0; index < MAX_X * MAX_Y; index++ )
	{
		LCD_WriteData(Color);
	}

}

static void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )
	
 	uint16_t temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;  

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )
		
	#endif

  switch( LCD_Code )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );     
          LCD_WriteReg(0x0021, Ypos );     
	      break; 

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );      
        LCD_WriteReg(0x004f, Ypos );          
	      break;  

     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );                                                  
	      LCD_WriteReg(0x03, Xpos );  

	      LCD_WriteReg(0x06, Ypos>>8 );                           
	      LCD_WriteReg(0x07, Ypos );    
	
	      break;     
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break; 
  }
}

/******************************************************************************
* Function Name  : LCD_BGR2RGB
* Description    : RRRRRGGGGGGBBBBB convert to BBBBBGGGGGGRRRRR
* Input          : RGB color
* Output         : None
* Return         : RGB color
* Attention		 :
*******************************************************************************/
static uint16_t LCD_BGR2RGB(uint16_t color)
{
	uint16_t  r, g, b, rgb;
	
	b = ( color>>0 )  & 0x1f;
	g = ( color>>5 )  & 0x3f;
	r = ( color>>11 ) & 0x1f;
	
	rgb =  (b<<11) + (g<<5) + (r<<0);
	
	return( rgb );
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : Get color of the point
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{
	uint16_t dummy;
	
	LCD_SetCursor(Xpos,Ypos);

	LCD_WriteIndex(0x0022);  
	
	switch( LCD_Code )
	{
		case ST7781:
		case LGDP4531:
		case LGDP4535:
		case SSD1289:
		case SSD1298:
      dummy = LCD_ReadData();
      dummy = LCD_ReadData(); 

 		  return  dummy;	      
    case HX8347A:
	  case HX8347D:
    {
      uint8_t red,green,blue;
      red = LCD_ReadData()>>3;
      green = LCD_ReadData()>>3; 
      blue = LCD_ReadData()>>2; 
      dummy = ( green << 11 ) | (blue << 5 ) | red;
		}
      return  dummy;
    default:	/* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x9919 */
      dummy = LCD_ReadData();
      dummy = LCD_ReadData(); 
      return  LCD_BGR2RGB( dummy );
	}
}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	if(LCD_Code==ILI9341){
		LCD_WR_REG(0x2a); 
		LCD_WR_DATA(Xpos>>8); 
		LCD_WR_DATA(Xpos&0XFF);	 
		LCD_WR_REG(0x2b); 
		LCD_WR_DATA(Ypos>>8); 
		LCD_WR_DATA(Ypos&0XFF);
		LCD_WriteIndex(0x002C);
			LCD_WriteData(point);
	}else{
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
	}
}



/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Bresenham's line algorithm
* Input          : - x0:
*                  - y0:
*       				   - x1:
*       		       - y1:
*                  - color:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color )
{
  short dx,dy;
  short temp;

  if( x0 > x1 )
  {
    temp = x1;
    x1 = x0;
    x0 = temp;   
  }
  if( y0 > y1 )
  {
    temp = y1;
    y1 = y0;
    y0 = temp;   
  }

  dx = x1-x0;
  dy = y1-y0;

  if( dx == 0 )
  {
    do
    { 
      LCD_SetPoint(x0, y0, color);
      y0++;
    }while( y1 >= y0 ); 
    return; 
  }
  if( dy == 0 )
  {
    do
    {
      LCD_SetPoint(x0, y0, color);
      x0++;
    }
    while( x1 >= x0 ); 
		return;
  }

	/* Bresenham's line algorithm  */
  if( dx > dy )
  {
    temp = 2 * dy - dx;
    while( x0 != x1 )
    {
	    LCD_SetPoint(x0,y0,color);
	    x0++;
	    if( temp > 0 )
	    {
	      y0++;
	      temp += 2 * dy - 2 * dx; 
	 	  }
      else         
      {
			  temp += 2 * dy;
			}       
    }
    LCD_SetPoint(x0,y0,color);
  }  
  else
  {
    temp = 2 * dx - dy;
    while( y0 != y1 )
    {
	 	  LCD_SetPoint(x0,y0,color);     
      y0++;                 
      if( temp > 0 )           
      {
        x0++;               
        temp+=2*dy-2*dx; 
      }
      else
			{
        temp += 2 * dy;
			}
    } 
    LCD_SetPoint(x0,y0,color);
	}
} 

/******************************************************************************
* Function Name  : PutChar
* Description    : 
* Input          : - Xpos:
*                  - Ypos:
*				           - ASCI:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void PutChar( uint16_t Xpos, uint16_t Ypos, uint8_t ASCI, uint16_t charColor, uint16_t bkColor )
{
	uint16_t i, j;
    uint8_t buffer[16], tmp_char;
    GetASCIICode(buffer,ASCI);
    for( i=0; i<16; i++ )
    {
        tmp_char = buffer[i];
        for( j=0; j<8; j++ )
        {
            if( (tmp_char >> 7 - j) & 0x01 == 0x01 )
            {
                LCD_SetPoint( Xpos + j, Ypos + i, charColor );
            }
            else
            {
                LCD_SetPoint( Xpos + j, Ypos + i, bkColor );
            }
        }
    }
}

/******************************************************************************
* Function Name  : GUI_Text
* Description    : 
* Input          : - Xpos: 
*                  - Ypos: 
*				           - str:
*				           - charColor:
*				           - bkColor:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor)
{
    uint8_t TempChar;
    do
    {
        TempChar = *str++;  
        PutChar( Xpos, Ypos, TempChar, Color, bkColor );    
        if( Xpos < MAX_X - 8 )
        {
            Xpos += 8;
        } 
        else if ( Ypos < MAX_Y - 16 )
        {
            Xpos = 0;
            Ypos += 16;
        }   
        else
        {
            Xpos = 0;
            Ypos = 0;
        }    
    }
    while ( *str != 0 );
}



void GLCD_SetWindow (unsigned int x, unsigned int y, unsigned int w, unsigned int h) {
  unsigned int xe, ye;

  if (0) {
    xe = x+w-1;
    ye = y+h-1;

    LCD_WriteReg(0x02, x  >>    8);           /* Column address start MSB           */
    LCD_WriteReg(0x03, x  &  0xFF);           /* Column address start LSB           */
    LCD_WriteReg(0x04, xe >>    8);           /* Column address end MSB             */
    LCD_WriteReg(0x05, xe &  0xFF);           /* Column address end LSB             */
  
    LCD_WriteReg(0x06, y  >>    8);           /* Row address start MSB              */
    LCD_WriteReg(0x07, y  &  0xFF);           /* Row address start LSB              */
    LCD_WriteReg(0x08, ye >>    8);           /* Row address end MSB                */
    LCD_WriteReg(0x09, ye &  0xFF);           /* Row address end LSB                */
  }
  else {
   //#if (LANDSCAPE == 1)
//     LCD_WriteReg(0x50, y);                    /* Vertical   GRAM Start Address      */
//     LCD_WriteReg(0x51, y+h-1);                /* Vertical   GRAM End   Address (-1) */
//     LCD_WriteReg(0x52, x);                    /* Horizontal GRAM Start Address      */
//     LCD_WriteReg(0x53, x+w-1);                /* Horizontal GRAM End   Address (-1) */
//     LCD_WriteReg(0x20, y);
//     LCD_WriteReg(0x21, x);
//    #else
    LCD_WriteReg(0x50, x);                    /* Horizontal GRAM Start Address      */
    LCD_WriteReg(0x51, x+w-1);                /* Horizontal GRAM End   Address (-1) */
    LCD_WriteReg(0x52, y);                    /* Vertical   GRAM Start Address      */
    LCD_WriteReg(0x53, y+h-1);                /* Vertical   GRAM End   Address (-1) */
    LCD_WriteReg(0x20, x);
    LCD_WriteReg(0x21, y);
//    #endif
  }
}

void GLCD_Bitmap (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap) {
  int i, j;
  unsigned short *bitmap_ptr = (unsigned short *)bitmap;

  GLCD_SetWindow (x, y, w, h);
//LCD_SetCursor(x,y);
	LCD_WriteIndex(0x0022);

  for (i = (h-1)*w; i > -1; i -= w) {
    for (j = 0; j < w; j++) {
      LCD_WriteData (bitmap_ptr[i+j]);
    }
  }

}


void GLCD_Bargraph (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val) {
  int i,j;

  val = (val * w) >> 10;                /* Scale value                        */
  GLCD_SetWindow(x, y, w, h);
  LCD_WriteIndex(0x22);

  for (i = 0; i < h; i++) {
    for (j = 0; j <= w-1; j++) {
      if(j >= val) {
        LCD_WriteData(Blue2);
      } else {
        LCD_WriteData(White);
      }
    }
  }

}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1,White);
	LCD_DrawLine(x1,y1,x1,y2,White);
	LCD_DrawLine(x1,y2,x2,y2,White);
	LCD_DrawLine(x2,y1,x2,y2,White);
}

void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_SetPoint(x0-b,y0-a,White);             //3           
		LCD_SetPoint(x0+b,y0-a,White);             //0           
		LCD_SetPoint(x0-a,y0+b,White);             //1       
		LCD_SetPoint(x0-b,y0-a,White);             //7           
		LCD_SetPoint(x0-a,y0-b,White);             //2             
		LCD_SetPoint(x0+b,y0+a,White);             //4               
		LCD_SetPoint(x0+a,y0-b,White);             //5
		LCD_SetPoint(x0+a,y0+b,White);             //6 
		LCD_SetPoint(x0-b,y0+a,White);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_SetPoint(x0+a,y0+b,White);
	}
} 

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteIndex(uint16_t index)
{
	LCD_REG	= index;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : - index:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteData(uint16_t data)
{
	LCD_RAM = data;
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
uint16_t LCD_ReadData(void)
{ 
  return LCD_RAM;
}
void LCD_WR_REG(uint16_t regval)
{   
	LCD_WriteIndex(regval);   
}
void LCD_WR_DATA(uint16_t data)
{	 
	LCD_WriteData(data);
}
/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{ 
	/* Write 16-bit Index, then Write Reg */  
	LCD_WriteIndex(LCD_Reg);         
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);  
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD_WriteIndex(LCD_Reg);

	/* Read 16-bit Reg */
	return LCD_ReadData();
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
