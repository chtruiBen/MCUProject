/**
  ******************************************************************************
  * @file     bsp_exti.c
  * @author  Louis
  * @version  V1.0
  * @date    2020.01.01
  * @brief    I/O中斷應用bsp
  ******************************************************************************

  ******************************************************************************
  */
  
#include "../key/bsp_exti.h"
//#include "gpio.h"
 /**
  */


/* USER CODE END 4 */
void ISL29023_INT_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 

    /*開啟按鍵GPIO埠的時鐘*/
    ISL29023_INT_GPIO_CLK_ENABLE()
    /* 選擇按鍵1的接腳 */ 
    GPIO_InitStructure.Pin = ISL29023_INT_GPIO_PIN;
    /* 設置接腳為輸入模式 */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;	    		
    /* 設置接腳不上拉也不下拉 */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* 使用上面的結構體初始化按鍵 */
    HAL_GPIO_Init(ISL29023_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* 配置 EXTI 中斷源 到key1 接腳、配置中斷優先順序*/
    HAL_NVIC_SetPriority(ISL29023_INT_EXTI_IRQ, 2, 0);
    /* 置能中斷 */
    HAL_NVIC_EnableIRQ(ISL29023_INT_EXTI_IRQ);

}
/*********************************************END OF FILE**********************/
