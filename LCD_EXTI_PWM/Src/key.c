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
  
#include "key.h"

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY0_INT_GPIO_PORT,KEY1_INT_GPIO_PORT,KEY2_INT_GPIO_PORT,
																			KEY3_INT_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {KEY0_INT_GPIO_PIN,KEY1_INT_GPIO_PIN,KEY2_INT_GPIO_PIN,KEY3_INT_GPIO_PIN}; 
 /**
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    /*開啟按鍵GPIO埠的時鐘*/
    KEY0_INT_GPIO_CLK_ENABLE();
    KEY1_INT_GPIO_CLK_ENABLE();
		KEY2_INT_GPIO_CLK_ENABLE();
		KEY3_INT_GPIO_CLK_ENABLE();

    /* 選擇按鍵1的接腳 */ 
    GPIO_InitStructure.Pin = KEY0_INT_GPIO_PIN;
    /* 設置接腳為輸入模式 */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
    /* 設置接腳不上拉也不下拉 */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* 使用上面的結構體初始化按鍵 */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* 配置 EXTI 中斷源 到key1 接腳、配置中斷優先順序*/
    HAL_NVIC_SetPriority(KEY0_INT_EXTI_IRQ, 0, 0);
    /* 置能中斷 */
    HAL_NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);

    /* 選擇按鍵2的接腳 */ 
    GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;  
    /* 其他配置與上面相同 */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* 配置 EXTI 中斷源 到key1 接腳、配置中斷優先順序*/
    HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ, 0, 0);
    /* 置能中斷 */
    HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
		
		
		
	  GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;  
    /* 其他配置與上面相同 */
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* 配置 EXTI 中斷源 到key1 接腳、配置中斷優先順序*/
    HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ, 0, 0);
    /* 置能中斷 */
    HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
		
		
		GPIO_InitStructure.Pin = KEY3_INT_GPIO_PIN;  
    /* 其他配置與上面相同 */
    HAL_GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* 配置 EXTI 中斷源 到key1 接腳、配置中斷優先順序*/
    HAL_NVIC_SetPriority(KEY3_INT_EXTI_IRQ, 0, 0);
    /* 置能中斷 */
    HAL_NVIC_EnableIRQ(KEY3_INT_EXTI_IRQ);
}

uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
	return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button] );
}


/*********************************************END OF FILE**********************/
