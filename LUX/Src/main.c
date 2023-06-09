/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
#include "bsp_led.h"
#include "bsP_i2c_ee.h"
#include "../key/bsp_exti.h"
#define MESSAGE1   "CYESS LUX RTC ALARM HW3" 
#define MESSAGE2   " Testing on  " 
#define MESSAGE3   "UART1" 


__IO uint8_t USARTFLAG=0;
__IO uint32_t 			TimingDelay = 0;


/*ISL29023*/
uint16_t luxRaw;
float lux;
 __IO uint8_t ISL29023_INT_FLAG=0;

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char ch;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure;
RTC_AlarmTypeDef salarmstructure;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	
	
	
uint8_t initISL29023(){
	uint8_t cmdData[1] ={0};
	cmdData[0] =0xA0;
	I2C_Write(ISL29023_ADDR,REGS_COMMAND_I,cmdData,1);

  cmdData[0] =0x03;
	I2C_Write(ISL29023_ADDR,REGS_COMMAND_II,cmdData,1);

  cmdData[0] =0x64;  //Set low threshlod 100lux
	I2C_Write(ISL29023_ADDR,0x04,cmdData,1);
	return 0;
}


uint8_t readLux(){
	uint8_t luxData[1] = {0};
	I2C_Read(ISL29023_ADDR,REGS_MBS_SENSOR,luxData,1);
	luxRaw = (luxData[0] << 8 );
	I2C_Read(ISL29023_ADDR,REGS_LBS_SENSOR,luxData,1);
	luxRaw |= luxData[0];
	lux = (64000 * (float)luxRaw)/65536;
	return 0;
}

int main(void)
{
char string[50];
	
	uint8_t i;
	uint8_t cmdData[1] ={0};
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
	I2C_EE_Init();
	LED_GPIO_Config();
  /* USER CODE BEGIN 2 */
	MX_RTC_Init();
	MX_Set_Alarm();
	MX_Set_Alarm_WakeUp();

	
  //I2C_LowLevel_Init();
	printf("\n\r %s", MESSAGE1);
  printf(" %s", MESSAGE2);
  printf(" %s\n\r", MESSAGE3);
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//ISL29023_INT_Configuration(); 
	initISL29023();
	LED0_OFF;
	LED1_OFF;
	LED2_OFF;
	LED3_OFF
	printf("RTC_BKP_DR0:%x\r\n",HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0));
	if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=RTC_BK_DATA){
			printf("\r\nFirst set RTC\r\n");
		//MX_RTC_Init();
	  MX_RTC_SetTime();
    MX_RTC_SetDate();
		HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,RTC_BK_DATA);
	}else{
		MX_RTC_Init();
	}
	
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if(ISL29023_INT_FLAG==1){
			
			ISL29023_INT_FLAG=0;			
			cmdData[0] =0x00;   //Clear int flag and Power down
			I2C_Write(ISL29023_ADDR,REGS_COMMAND_I,cmdData,1);
			
			//HAL_Delay(100);
			cmdData[0] =0xA0;  //Enable chip
			I2C_Write(ISL29023_ADDR,REGS_COMMAND_I,cmdData,1);
			
		}else{
			//readLux();
			HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
		/* Get the RTC current Date */
		HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);
			printf("%02d/%02d/%02d\r\n",2000 + sdatestructure.Year, sdatestructure.Month, sdatestructure.Date); 
		/* Display time Format : hh:mm:ss */
		printf("%02d:%02d:%02d\r\n",stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds);

		printf("\r\n");	
			HAL_Delay(2000);
	    printf("wait\n\r");
		}
		
		HAL_Delay(100);
  }
  /* USER CODE END 3 */
}





/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Supply configuration update enable 
  */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  /**Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
  {
    
  }
	/**Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void MX_Set_Alarm(void)
{
  /*#################  Configure the RTC Alarm peripheral #################*/
  salarmstructure.Alarm = RTC_ALARM_B;
  salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY |RTC_ALARMMASK_HOURS |RTC_ALARMMASK_MINUTES;
  salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
  salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  salarmstructure.AlarmTime.Hours   = 0x02;
  salarmstructure.AlarmTime.Minutes = 0x34;
  salarmstructure.AlarmTime.Seconds = 0x00;
  salarmstructure.AlarmTime.SubSeconds = 00;    

  HAL_RTC_SetAlarm_IT(&hrtc,&salarmstructure,RTC_FORMAT_BCD);
	salarmstructure.Alarm = RTC_ALARM_A;
	salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY |RTC_ALARMMASK_HOURS ;
	salarmstructure.AlarmTime.Minutes = 0x00;
	salarmstructure.AlarmTime.Seconds = 0x30;
	HAL_RTC_SetAlarm_IT(&hrtc,&salarmstructure,RTC_FORMAT_BCD);
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 1);
    /* ¸m¯à¤¤Â_ */
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);

}
void MX_Set_Alarm_WakeUp()
{
	 HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,29,RTC_WAKEUPCLOCK_CK_SPRE_16BITS); // every 5s 
	 
	
	 HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Turn LED2 on: Alarm generation */
LED0_TOGGLE;

LED1_TOGGLE;
printf("alarm A/HR\n\r");
 // * @brief  This function is executed i

}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* Turn LED2 on: Alarm generation */
LED2_TOGGLE;

LED3_TOGGLE;
printf("alarm B/MIN\n\r");
 // * @brief  This function is executed i

}
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	printf("GIVE ME THE LUX ~~~~\r\n");
	readLux();
	printf("ISL29023 Lux: %.0f\r\n",lux);
//LED0(ON);
//LED1(OFF);
//LED2(ON);
//LED3(OFF);
	
}
/* USER CODE END 4 */
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
