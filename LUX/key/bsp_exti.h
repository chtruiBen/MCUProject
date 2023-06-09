#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32h7xx.h"


/*******************************************************/
#define KEY0_INT_GPIO_PORT                GPIOA
#define KEY0_INT_GPIO_CLK_ENABLE()        __GPIOA_CLK_ENABLE();
#define KEY0_INT_GPIO_PIN                 GPIO_PIN_0
#define KEY0_INT_EXTI_IRQ                 EXTI0_IRQn
#define KEY0_IRQHandler                   EXTI0_IRQHandler

#define KEY1_INT_GPIO_PORT                GPIOE
#define KEY1_INT_GPIO_CLK_ENABLE()        __GPIOE_CLK_ENABLE();
#define KEY1_INT_GPIO_PIN                 GPIO_PIN_2
#define KEY1_INT_EXTI_IRQ                 EXTI2_IRQn
#define KEY1_IRQHandler                   EXTI2_IRQHandler


#define KEY2_INT_GPIO_PORT                GPIOE
#define KEY2_INT_GPIO_CLK_ENABLE()        __GPIOE_CLK_ENABLE();
#define KEY2_INT_GPIO_PIN                 GPIO_PIN_3
#define KEY2_INT_EXTI_IRQ                 EXTI3_IRQn
#define KEY2_IRQHandler                   EXTI3_IRQHandler



#define KEY3_INT_GPIO_PORT                GPIOE
#define KEY3_INT_GPIO_CLK_ENABLE()        __GPIOE_CLK_ENABLE();
#define KEY3_INT_GPIO_PIN                 GPIO_PIN_4
#define KEY3_INT_EXTI_IRQ                 EXTI4_IRQn
#define KEY3_IRQHandler                   EXTI4_IRQHandler


#define ISL29023_INT_GPIO_PORT                GPIOG
#define ISL29023_INT_GPIO_CLK_ENABLE()        __GPIOG_CLK_ENABLE();
#define ISL29023_INT_GPIO_PIN                 GPIO_PIN_8
#define ISL29023_INT_EXTI_IRQ                 EXTI9_5_IRQn
#define ISL29023_INT_IRQHandler               EXTI9_5_IRQHandler










/*******************************************************/


void EXTI_Key_Config(void);
void ISL29023_INT_Configuration(void);

#endif /* __EXTI_H */
