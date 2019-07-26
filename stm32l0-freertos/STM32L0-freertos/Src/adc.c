
#include "adc.h"
#include "stdio.h"
#include "stm32l0xx_nucleo_32.h"

/* ADC handler declaration */
ADC_HandleTypeDef    AdcHandle;

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;

/* Variable used to get converted value */
__IO uint32_t uwADCxConvertedValue = 0;

void ADC_init(void)
{
  
    AdcHandle.Instance = ADC1;

  AdcHandle.Init.OversamplingMode      = DISABLE;

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV1;
  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
  AdcHandle.Init.LowPowerFrequencyMode = ENABLE;
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
    
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.SamplingTime          = ADC_SAMPLETIME_7CYCLES_5;
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.ContinuousConvMode    = ENABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  
    /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
//    Error_Handler();
  }


  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) != HAL_OK)
  {
//    Error_Handler();
  }

  /* ### - 3 - Channel configuration ######################################## */
  /* Select Channel 0 to be converted */
  sConfig.Channel = ADC_CHANNEL_0;    
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
//    Error_Handler();
  }

 /*##- 4- Start the conversion process #######################################*/  
  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
//    Error_Handler();
  }
  
    /* Infinite loop */
  while (1)
  {
    /*##- 5- Wait for the end of conversion #####################################*/  
    /*  Before starting a new conversion, you need to check the current state of
         the peripheral; if it’s busy you need to wait for the end of current
         conversion before starting a new one.
         For simplicity reasons, this example is just waiting till the end of the
         conversion, but application may perform other tasks while conversion
         operation is ongoing. */
    if (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK)
    {
      /* End Of Conversion flag not set on time */
//      Error_Handler();
    }

    /* Check if the continuous conversion of regular channel is finished */
    if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
    {
        /*##-6- Get the converted value of regular channel  ########################*/
        uwADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
        printf("%d\r\n", uwADCxConvertedValue);
    }
    
    printf("find\r\n");                                                             
    BSP_LED_Toggle(LED3);
    HAL_Delay(1000);
  }
}

