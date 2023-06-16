
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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "tim.h"
#include "rtc.h"
#include "bsp_basic_tim.h"
#include "usart.h"
#include "stm32h7xx_hal.h"

#include "lcd.h"
#include "i2c.h"
#include "key.h"

#define MESSAGE1   "CYESS Lora Example" 
#define MESSAGE2   " Testing on  " 
#define MESSAGE3   "   01" 
#define MESSAGE   "CYESS CYE-STM32H743-C Demo" 
/* USER CODE BEGIN Includes */

/*LM75A*/
float readtemp() {
	uint8_t tempData[2] = {0};
	I2C_Read(LM75A_ADDR,LM75A_REG_READ_TEMP,tempData,2);
	uint16_t tempRaw = tempData[1]|(tempData[0] << 8);
	tempRaw = tempRaw>>5;
	
  float res = 0.f;
	
  res = tempRaw * 0.125f;

  if ((tempData[0] & 0x80) == 1) {
		res *= -1;
	}
	return res;
}

/*ISL29023*/
#define ISL29023_ADDR          0x89
#define REGS_COMMAND_I          0x0
#define REGS_COMMAND_II         0x1
#define REGS_LBS_SENSOR         0x2
#define REGS_MBS_SENSOR         0x3

void initISL29023() {
  uint8_t cmdData[1] = { 0 };
  cmdData[0] = 0xA0;
  I2C_Write(ISL29023_ADDR, REGS_COMMAND_I, cmdData, 1);

  cmdData[0] = 0x03;
  I2C_Write(ISL29023_ADDR, REGS_COMMAND_II, cmdData, 1);

  // cmdData[0] = 0xF4;  //Set low threshlod 100lux
  // I2C_Write(ISL29023_ADDR, 0x04, cmdData, 1);
}

float readLux(){
  uint8_t luxData[1] = { 0 };
  I2C_Read(ISL29023_ADDR, REGS_MBS_SENSOR, luxData, 1);
  uint16_t luxRaw = (luxData[0] << 8);
  I2C_Read(ISL29023_ADDR, REGS_LBS_SENSOR, luxData, 1);
  luxRaw |= luxData[0];
  float lux = (64000 * (float)luxRaw) / 65536;
  return lux;
}

void SystemClock_Config(void);

void MX_GPIO_Init();
void LCD_Show();

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

RTC_AlarmTypeDef salarmstructure;
void MX_Set_Alarm(void)
{
  salarmstructure.Alarm = RTC_ALARM_A;
  salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
  salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
  salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
  salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT_24;
  salarmstructure.AlarmTime.Hours = 0x7;
  salarmstructure.AlarmTime.Minutes = 0;
  salarmstructure.AlarmTime.Seconds = 0;
  salarmstructure.AlarmTime.SubSeconds = 0;
  HAL_RTC_SetAlarm_IT(&hrtc, &salarmstructure, RTC_FORMAT_BCD);

  salarmstructure.Alarm = RTC_ALARM_B;
  salarmstructure.AlarmTime.Hours = 0x18;
  salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  HAL_RTC_SetAlarm_IT(&hrtc, &salarmstructure, RTC_FORMAT_BCD);

  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
typedef enum Mode {SHOW_LUX , PWM} Mode_t;
Mode_t mode = SHOW_LUX;

typedef enum Sensor { LM75A, ISL29023, STOP } Sensor_t;
Sensor_t sensor = STOP;

const int feature1_index = 0;
const int feature2_index = 6;
const int feature1_lux_index = 5;

int16_t led_brightness = 800;

const char flag = '<';

char str[11][45] = {
  "1. Show lux: <",
  " K1--> LM75A",
  " K2--> ISL29023",
  " K3--> Stop showing and switch to \"PWM\"",
  " K4--> Reset",
  " ",
  "2. PWM(LED2):  ",
  " K1--> Increase brightness",
  " K2--> Decrease brightness",
  " K3--> Reset",
  " K4--> Switch to \"Show lux\""
};

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	uint8_t buf[128];

  MPU_Config();
  
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
	MX_USART1_UART_Init();

  MX_TIM3_Init();
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

  MX_RTC_Init();
  MX_Set_Alarm();

  TIM_Basic_Init();

  I2C_EE_Init();
  initISL29023();
	EXTI_Key_Config(); 

	LCD_Initializtion();
	LCD_Clear(Black);
		
	printf("\n\r %s", MESSAGE1);
  printf(" %s", MESSAGE2);
  printf(" %s\r\n", MESSAGE3);

  LCD_Show();

  for(;;)
  {
    switch (sensor) {
    case LM75A:
      sprintf(str[feature1_lux_index], "%s%f", " LM75A: ", readtemp());
      LCD_Show();
      break;
    case ISL29023:
      sprintf(str[feature1_lux_index], "%s%f", " ISL29023: ", readLux());
      LCD_Show();
      break;
    case STOP:
      break;
    default:
      break;
    }
    HAL_Delay(500);
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, 1);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1);
}

void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, 0);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
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
    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  // PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(SystemCoreClock/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);	
												   
	GPIO_InitStruct.Pin = GPIO_PIN_6;	
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, 1);
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1);
}

void LCD_Show() {
  LCD_Clear(Black);
  static const uint16_t o_x = 5;
  static const uint16_t o_y = 10;

  static const uint16_t offset = 20;

  for (int i = 0; i < 11; i++) {
    GUI_Text(o_x, o_y + offset * i, str[i], Yellow, Black);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void ModifyBrightness() {
  if (led_brightness < 800) {
      led_brightness = 800;
    }
    else if (led_brightness > 1001) {
      led_brightness = 1001;
    }
    TIM3->CCR1 = led_brightness;
}

void Key1Callback() {
  if (mode == SHOW_LUX) {
    sensor = LM75A;
  }
  else {
    led_brightness -= 10;
    ModifyBrightness();
  }
}

void Key2Callback() {
  if (mode == SHOW_LUX) {
    sensor = ISL29023;
  }
  else {
    led_brightness += 10;
    ModifyBrightness();
  }
}
void Key3Callback() {
  if (mode == SHOW_LUX) {
    sensor = STOP;
    str[feature1_index][strlen(str[feature1_index]) - 1] = ' ';
    str[feature2_index][strlen(str[feature2_index]) - 1] = flag;
    mode = PWM;
    memmove(str[feature1_lux_index], " ", 40);
    LCD_Show();
  }
  else {
    NVIC_SystemReset();
  }
}
void Key4Callback() {
  if (mode == SHOW_LUX) {
    NVIC_SystemReset();
  }
  else {
    str[feature1_index][strlen(str[feature1_index]) - 1] = flag;
    str[feature2_index][strlen(str[feature2_index]) - 1] = ' ';
    mode = SHOW_LUX;
    LCD_Show();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
