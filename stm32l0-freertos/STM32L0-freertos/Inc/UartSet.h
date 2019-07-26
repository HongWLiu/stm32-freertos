#ifndef __UART_GPIO_H
#define __UART_GPIO_H
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_32.h"


//模拟选择的GPIO接口：发送
#define     COM_TX_GPIO_Port    GPIOA
#define     COM_TX_Pin          GPIO_PIN_4

#define     COM_RX_GPIO_Port    GPIOA
#define     COM_RX_Pin          GPIO_PIN_5

/* Definition for TIMx clock resources */
#define TIMx                           TIM2
#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM2_CLK_ENABLE()


/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM2_IRQn
#define TIMx_IRQHandler                TIM2_IRQHandler



typedef enum {FALSE = 0, TRUE = !FALSE} bool;

void UART_Init(void);

void deal_sampling_data(uint8_t *data);

#endif
