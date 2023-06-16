/**
  ******************************************************************************
  * @file     bsp_exti.c
  * @author  Louis
  * @version  V1.0
  * @date    2020.01.01
  * @brief    I/O���_����bsp
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

    /*�}�ҫ���GPIO�𪺮���*/
    KEY0_INT_GPIO_CLK_ENABLE();
    KEY1_INT_GPIO_CLK_ENABLE();
		KEY2_INT_GPIO_CLK_ENABLE();
		KEY3_INT_GPIO_CLK_ENABLE();

    /* ��ܫ���1�����} */ 
    GPIO_InitStructure.Pin = KEY0_INT_GPIO_PIN;
    /* �]�m���}����J�Ҧ� */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	    		
    /* �]�m���}���W�Ԥ]���U�� */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* �ϥΤW�������c���l�ƫ��� */
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* �t�m EXTI ���_�� ��key1 ���}�B�t�m���_�u������*/
    HAL_NVIC_SetPriority(KEY0_INT_EXTI_IRQ, 0, 0);
    /* �m�त�_ */
    HAL_NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);

    /* ��ܫ���2�����} */ 
    GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;  
    /* ��L�t�m�P�W���ۦP */
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* �t�m EXTI ���_�� ��key1 ���}�B�t�m���_�u������*/
    HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ, 0, 0);
    /* �m�त�_ */
    HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
		
		
		
	  GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;  
    /* ��L�t�m�P�W���ۦP */
    HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* �t�m EXTI ���_�� ��key1 ���}�B�t�m���_�u������*/
    HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ, 0, 0);
    /* �m�त�_ */
    HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
		
		
		GPIO_InitStructure.Pin = KEY3_INT_GPIO_PIN;  
    /* ��L�t�m�P�W���ۦP */
    HAL_GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);      
    /* �t�m EXTI ���_�� ��key1 ���}�B�t�m���_�u������*/
    HAL_NVIC_SetPriority(KEY3_INT_EXTI_IRQ, 0, 0);
    /* �m�त�_ */
    HAL_NVIC_EnableIRQ(KEY3_INT_EXTI_IRQ);
}

uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
	return HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button] );
}


/*********************************************END OF FILE**********************/
