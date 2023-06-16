/**
  *********************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2020.02.08
  * @brief   i2c EEPROM(AT24C02)
  *********************************************************************  
  */ 

#include "i2c.h"
#include "usart.h"


I2C_HandleTypeDef  I2C_Handle; 

/**
  * @brief  I2C1 I/O�t�m
  * @param  �L
  * @retval �L
  */
static void I2C_GPIO_Config(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	  
	/*�P��I2C clock*/
	EEPROM_I2C_CLK_ENABLE();

	/*�P��I2C��IO */
	EEPROM_I2C_SCL_GPIO_CLK_ENABLE();
	EEPROM_I2C_SDA_GPIO_CLK_ENABLE(); 

	/*�t�mI2C��SCL��*/   
	GPIO_InitStructure.Pin = EEPROM_I2C_SCL_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Alternate = EEPROM_I2C_SCL_AF;
	HAL_GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	/*�t�mI2C��SDA��*/  
	GPIO_InitStructure.Pin = EEPROM_I2C_SDA_PIN;
	HAL_GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Force the I2C peripheral clock reset */  
	EEPROM_I2C_FORCE_RESET(); 

	/* Release the I2C peripheral clock reset */  
	EEPROM_I2C_RELEASE_RESET(); 

}

/**
  * @brief  I2C �u�@�Ҧ��t�m
  */
static void I2C_Mode_Config(void)
{
    /* I2C �t�m */
    I2C_Handle.Instance = EEPROM_I2C;
    I2C_Handle.Init.Timing           = 0x40604E73;//100KHz
    I2C_Handle.Init.OwnAddress1      = 0;
    I2C_Handle.Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    I2C_Handle.Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    I2C_Handle.Init.OwnAddress2      = 0;
    I2C_Handle.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    I2C_Handle.Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    I2C_Handle.Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    /* Init the I2C */
    HAL_I2C_Init(&I2C_Handle);	

    HAL_I2CEx_AnalogFilter_Config(&I2C_Handle, I2C_ANALOGFILTER_ENABLE);    
}

/**
  * @brief  I2C �~���Ҳժ�(EEPROM)��l��
  */
void I2C_EE_Init(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config();
}

/**
  * @brief   �N�w�İϤ�����Ƽg��I2C EEPROM��
  * @param   
  *		@arg pBuffer:�w�İϫ���
  *		@arg WriteAddr:�g�a�}
  *     @arg NumByteToWrite:�g���줸�ռ�
  */
void I2C_EE_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % EEPROM_PAGESIZE;
  count = EEPROM_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
  NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE); 
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
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
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / EEPROM_PAGESIZE;
      NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, EEPROM_PAGESIZE);
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
      }
    }
  }  
}

/**
  * @brief   �g�@�Ӧ줸�ը�I2C EEPROM��
  * @param   
  *		@arg pBuffer:�w�İϫ���
  *		@arg WriteAddr:�g�a�} 
  */
uint32_t I2C_EE_ByteWrite(uint8_t* pBuffer, uint8_t WriteAddr)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&I2C_Handle, EEPROM_ADDRESS, (uint16_t)WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, 1, 100); 

	/* Check the communication status */
	if(status != HAL_OK)
	{
	/* Execute user timeout callback */
	//I2Cx_Error(Addr);
	}
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	while (HAL_I2C_IsDeviceReady(&I2C_Handle, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   �bEEPROM���@�Ӽg�j�餤�i�H�g�h�Ӧ줸�աA���@���g�J���줸�ռƤ���W�LEEPROM�����j�p�AAT24C02�C����8�Ӧ줸��
  * @param   
  *		@arg pBuffer:�w�İϫ���
  *		@arg WriteAddr:�g�a�}
  *     @arg NumByteToWrite:�g���줸�ռ�
  */
uint32_t I2C_EE_PageWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status = HAL_OK;
	/* Write EEPROM_PAGESIZE */
	status=HAL_I2C_Mem_Write(&I2C_Handle, EEPROM_ADDRESS,WriteAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)(pBuffer),NumByteToWrite, 100);

	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	while (HAL_I2C_IsDeviceReady(&I2C_Handle, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

/**
  * @brief   �qEEPROM�̭�Ū���@�ϸ�� 
  * @param   
  *		@arg pBuffer:�s��qEEPROMŪ������ƪ��w�İϫ���
  *		@arg WriteAddr:������ƪ�EEPROM����}
  *     @arg NumByteToWrite:�qEEPROMŪ�����줸�ռ�
  * @retval  �L
  */
uint32_t I2C_EE_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status=HAL_I2C_Mem_Read(&I2C_Handle,EEPROM_ADDRESS,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead,1000);

	return status;
}
//uint8_t I2C_Write(uint8_t I2C_Addr,uint8_t addr,const uint8_t* buf,  uint32_t nbyte)
	
uint32_t I2C_Write(uint8_t I2C_Addr, uint8_t WriteAddr,uint8_t* pBuffer,  uint32_t nbyte)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(&I2C_Handle, I2C_Addr, (uint16_t)WriteAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, nbyte, 100); 

	/* Check the communication status */
	if(status != HAL_OK)
	{
	/* Execute user timeout callback */
	//I2Cx_Error(Addr);
	}
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}

	/* Check if the EEPROM is ready for a new operation */
	// while (HAL_I2C_IsDeviceReady(&I2C_Handle, EEPROM_ADDRESS, EEPROM_MAX_TRIALS, I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);

	/* Wait for the end of the transfer */
	while (HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}

uint32_t I2C_Read(uint8_t I2C_Addr,uint8_t ReadAddr,uint8_t* pBuffer,  uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status=HAL_I2C_Mem_Read(&I2C_Handle,I2C_Addr,ReadAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pBuffer, NumByteToRead,1000);
//printf("%d\r\n",status);
	return status;
}


