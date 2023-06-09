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
  
#include "../key/bsp_exti.h"
//#include "gpio.h"
 /**
  */


/* USER CODE END 4 */
void ISL29023_INT_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 

    /*�}�ҫ���GPIO�𪺮���*/
    ISL29023_INT_GPIO_CLK_ENABLE()
    /* ��ܫ���1�����} */ 
    GPIO_InitStructure.Pin = ISL29023_INT_GPIO_PIN;
    /* �]�m���}����J�Ҧ� */ 
    GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;	    		
    /* �]�m���}���W�Ԥ]���U�� */
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /* �ϥΤW�������c���l�ƫ��� */
    HAL_GPIO_Init(ISL29023_INT_GPIO_PORT, &GPIO_InitStructure); 
    /* �t�m EXTI ���_�� ��key1 ���}�B�t�m���_�u������*/
    HAL_NVIC_SetPriority(ISL29023_INT_EXTI_IRQ, 2, 0);
    /* �m�त�_ */
    HAL_NVIC_EnableIRQ(ISL29023_INT_EXTI_IRQ);

}
/*********************************************END OF FILE**********************/
