#include "UartSet.h"
#include "string.h"
#include "stdio.h"


uint32_t ucTim2Flag;

 /* TIM handle declaration */
 TIM_HandleTypeDef    TimHandle;

 /* Prescaler declaration */


 
/**
  * @brief  定时器2的初始化配置函数
  * @param  None:配置时钟频率是1MHZ，模拟的波特率是：9600；计数值是：104；
  * @retval None
  */
static void Timer2_Init(void)
{ 
   uint32_t uwPrescalerValue = 0;
  /*##-1- Enable peripheral clock #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();
  
  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);
  
    
  /* Compute the prescaler value to have TIMx counter clock equal to 1 MHz */
  uwPrescalerValue = (uint32_t)(SystemCoreClock / 1000000) - 1;

  printf("%d\n", SystemCoreClock);
    
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;


  TimHandle.Init.Period            = 500 - 1;               //0.5ms中断一次
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    
  
  HAL_TIM_Base_Init(&TimHandle);

  HAL_TIM_Base_Start_IT(&TimHandle);
}

/**
  * @brief  PA4,PA5,端口模拟GPIO发送
  * @param  None
  * @retval None
  */
static void UART_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
    
  __HAL_RCC_GPIOA_CLK_ENABLE();
       
          /* Configure PA.5:配置为RX*/
  GPIO_InitStruct.Pin = COM_RX_Pin;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;

  HAL_GPIO_Init(COM_RX_GPIO_Port, &GPIO_InitStruct);
     
}

/**
  * @brief  使能模拟的串口的GPIO和定时器
  * @param  None
  * @retval None
  */

void UART_Init(void)
{
    
    UART_GPIO_Init();
        
    Timer2_Init();
}




/**
  * @brief  定时器的功能函数：处理字符串位的发送。
  * @param  htim : TIM handle
  * @retval None
  */
uint8_t Pin_buffer[160] = {0};

void TIM2_IRQHandler(void)
{
    
    HAL_TIM_IRQHandler(&TimHandle);

    Pin_buffer[ucTim2Flag++] = HAL_GPIO_ReadPin(COM_RX_GPIO_Port, COM_RX_Pin);   
  
    if(ucTim2Flag >= 160)
    {
        HAL_TIM_Base_Stop_IT(&TimHandle);
//        printf("test:%d\r\n", ucTim2Flag);
    }

}


/*以1ms的定时器中断去采集GPIO的电平，获取1秒钟的采样数据，存在buffer中*/

void deal_sampling_data(uint8_t *data)
{
    uint16_t i=0,j = 0,k = 0;
    

    uint8_t temp[20] = {0,};
  
    if(ucTim2Flag >= 160)  //判断采集完成
    {
        while(i < 160)
        {
          temp[j] |= data[i] << k;
          k++;
          i++;
          
          if(k%8 == 0)
          {
            k = 0;
//            printf("0x%x ", temp[j]);
            j++;
          }
          
        }
        
        j = 0;
        while(temp[j] != 0x0)
        {
          j++;
          if(j >= 19)
            break;
        }
        
        if(j >= 19)
        {
        }
        else
        {
          printf("find\r\n");
        }

           
       ucTim2Flag = 0;  
       memset(data, 0, sizeof(Pin_buffer));
       HAL_TIM_Base_Start_IT(&TimHandle); //开启下一次采集
   }
}



  