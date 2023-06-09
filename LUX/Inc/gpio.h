/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif
//UINT1  PG8
//UINT2 PG9
#define LM75A_OS_PIN                   GPIO_Pin_8
#define LM75A_OS_GPIO_PORT             GPIOG
#define LM75A_OS_GPIO_CLK              RCC_AHB1Periph_GPIOG 
#define LM75A_OS_EXTI_LINE             EXTI_Line8
#define LM75A_OS_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOG
#define LM75A_OS_EXTI_PIN_SOURCE       EXTI_PinSource8
#define LM75A_OS_EXTI_IRQn             EXTI9_5_IRQn 

#define LM75A_ADDR 0x90
#define LM75A_REG_READ_TEMP	0x00
#define LM75A_REG_READ_THYST	0x02
#define LM75A_REG_READ_TOS	0x03






//UINT1  PG8
//UINT2 PG9
#define ISL29023_INT_PIN               GPIO_PIN_8
#define ISL29023_GPIO_PORT             GPIOG
#define ISL29023_GPIO_CLK              __GPIOG_CLK_ENABLE();
#define ISL29023_EXTI_LINE             EXTI_Line8
#define ISL29023_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOG
#define ISL29023_EXTI_PIN_SOURCE       EXTI_PinSource8 
#define ISL29023_INT_EXTI_IRQ          EXTI9_5_IRQn 
#define ISL29023_IRQHandler            EXTI9_5_IRQHandler

#define ISL29023_ADDR          0x89
#define REGS_COMMAND_I          0x0
#define REGS_COMMAND_II         0x1
#define REGS_LBS_SENSOR         0x2
#define REGS_MBS_SENSOR         0x3


/*BMA150*/
#define BMA150_ADDR 0x70
#define BMA150_REG_READ_X	0x02
#define BMA150_REG_READ_Y	0x04
#define BMA150_REG_READ_Z	0x06
#define BMA150_REG_READ_temp 0x08
#define BMA150_REG_RANGE  0x14

/*SHT20*/
#define MEASUREMENT_T_POLL  0xE3
#define MEASUREMENT_RH_POLL 0xE5
#define SHT20_MEAS_TIME_HUMIDITY                29
#define SHT20_MEAS_TIME_TEMPERATURE                85
#define SHT20_STATUS_BITS_MASK                        0x03
#define SHT20_RESULT_TYPE                        0x02
#define SHT20_TEMPERATURE_RESULT                0x00
#define SHT20_HUMIDITY_RESULT                        0x02
#define SHT20_ADDR 0x81

/*BMP180*/
#define BMP180_ADDR							0x77 <<1
#define BMP180_PROM_START_ADDR  0xAA
#define BMP180_PROM_DATA_LEN    22
#define BMP180_T_MEASURE        0x2E    // temperature measurement
#define BMP180_P_MEASURE        0x34 + (3<<6) // pressure measurement
#define BMP180_CTRL_MEAS_REG    0xF4
#define BMP180_ADC_OUT_MSB_REG  0xF6


/**
  * @brief  SD FLASH SDIO Interface
  */
#define SD_DETECT_PIN                    GPIO_Pin_15                 /* PA.15 */
#define SD_DETECT_GPIO_PORT              GPIOA                     /* GPIOA */
#define SD_DETECT_GPIO_CLK               RCC_AHB1Periph_GPIOA

#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)	
	
	/**
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
/**
  * @brief  SDIO Data Transfer Frequency (25MHz max)
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x0)

#define SD_SDIO_DMA                   DMA2
#define SD_SDIO_DMA_CLK               RCC_AHB1Periph_DMA2

#define SD_SDIO_DMA_STREAM3	          3
//#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
 #define SD_SDIO_DMA_STREAM            DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF3
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF3
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF3
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF3
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF3
 #define SD_SDIO_DMA_IRQn              DMA2_Stream3_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM            DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL           DMA_Channel_4
 #define SD_SDIO_DMA_FLAG_FEIF         DMA_FLAG_FEIF6
 #define SD_SDIO_DMA_FLAG_DMEIF        DMA_FLAG_DMEIF6
 #define SD_SDIO_DMA_FLAG_TEIF         DMA_FLAG_TEIF6
 #define SD_SDIO_DMA_FLAG_HTIF         DMA_FLAG_HTIF6
 #define SD_SDIO_DMA_FLAG_TCIF         DMA_FLAG_TCIF6
 #define SD_SDIO_DMA_IRQn              DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */



/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */




	
	


/**

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
