/**
  ******************************************************************
  * @file    bsp_led.c
  * @author  Louis
  * @version V1.0
  * @date    2020.01.01
  * @brief   led應用函數介面
  ******************************************************************
  */  
  
#include "bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  無
  * @retval 無
  */
void LED_GPIO_Config(void)
{		
	/*定義一個GPIO_InitTypeDef類型的結構*/
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*開啟LED相關的GPIO外設時鐘*/
	LED0_GPIO_CLK_ENABLE();
	LED1_GPIO_CLK_ENABLE();
	LED2_GPIO_CLK_ENABLE();
	LED3_GPIO_CLK_ENABLE();

	/*選擇要控制的GPIO接腳*/															   
	GPIO_InitStruct.Pin = LED0_PIN;	

	/*設置接腳的輸出類型為推挽輸出*/
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

	/*設置接腳為上拉模式*/
	GPIO_InitStruct.Pull  = GPIO_PULLUP;

	/*設置接腳速率為高速 */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 

	/*調用庫函數，使用上面配置的GPIO_InitStructure初始化GPIO*/
	HAL_GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStruct);	

		/*選擇要控制的GPIO接腳*/															   
	GPIO_InitStruct.Pin = LED1_PIN;	
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);

	/*選擇要控制的GPIO接腳*/															   
	GPIO_InitStruct.Pin = LED2_PIN;	
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);	
	

	/*選擇要控制的GPIO接腳*/															   
	GPIO_InitStruct.Pin = LED3_PIN;	
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);	

	/*關閉RGB燈*/
	LED_RGBOFF;
		
}
/*********************************************END OF FILE**********************/
