#include "bsp_basic_tim.h"
TIM_HandleTypeDef TIM_Base;


 /**
  * @brief  �򥻭p�ɾ� TIMx,x[6,7]���_�u�����ǰt�m
  * @param  �L
  * @retval �L
  */
static void TIMx_NVIC_Configuration(void)
{
    HAL_NVIC_SetPriority(BASIC_TIM_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(BASIC_TIM_IRQn);
}
/*
 * �`�N�GTIM_TimeBaseInitTypeDef���c��̭���5�Ӧ����ATIM6�MTIM7���Ȧs���̭��u��
 * TIM_Prescaler�MTIM_Period�A�ҥH�ϥ�TIM6�MTIM7���ɭԥu�ݪ�l�Ƴo��Ӧ����Y�i�A
 * �t�~�T�Ӧ����O�q�έp�ɾ��M���ŭp�ɾ��~��.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]�S���A��L�����]�򥻭p�ɾ��^
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]�S���A��L����(�򥻭p�ɾ�)
 * TIM_RepetitionCounter TIMx,x[1,8]�~��(���ŭp�ɾ�)
 *-----------------------------------------------------------------------------
 */
static void TIMx_Configuration(void)
{
    BASIC_TIM_CLK_ENABLE();
     
    TIM_Base.Instance = BASIC_TIM;
    /* �֭p TIM_Period�ӫᲣ�ͤ@�ӧ�s�Ϊ̤��_*/		
    //���p�ɾ��q0�p�ƨ�4999�A�Y��5000���A���@�өw�ɶg��
	
    TIM_Base.Init.Period = 5000 - 1;
    //�p�ɾ�������TIMxCLK = 2 * PCLK1  
    //				PCLK1 = HCLK / 4 
    //				=> TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
    // �]�w�p�ɾ��W�v��=TIMxCLK/(TIM_Prescaler+1)=10000Hz
	
		
    TIM_Base.Init.Prescaler =  20000 - 1;

	
    // ��l�ƭp�ɾ�TIM
    HAL_TIM_Base_Init(&TIM_Base);
    // �}�ҭp�ɾ���s���_
    HAL_TIM_Base_Start_IT(&TIM_Base);
}
/**
  * @brief  ��l�ư򥻭p�ɾ��w�ɡA1ms���ͤ@�����_
  * @param  �L
  * @retval �L
  */
void TIM_Basic_Init(void)
{
  TIMx_Configuration();
  TIMx_NVIC_Configuration();
}
/**
  * @brief  �p�ɾ���s���_Callback���
  * @param  htim : TIM����X
  * @retval �L
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim->Instance == TIM6) {
    static short b = 1001;
    b -= 100;
    if(b < 0) {
      b = 1001;
    }
    TIM3->CCR2 = b;
  }
}


/*********************************************END OF FILE**********************/

