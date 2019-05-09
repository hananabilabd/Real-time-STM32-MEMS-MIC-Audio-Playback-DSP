#include "main.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "i2s.h"
#include "pdm2pcm.h"
#include "gpio.h"

#define size_i2s_buffer  32 //=32
#define size_pdm_buffer  64
#define size_pcm_buffer  8

#define AUDIO_CHANNELS         1
#define MAX_MIC_FREQ           3072
#define N_MS_PER_INTERRUPT     1
#define N_MS                   1
#define PDM_Clock_Freq         3072
#define AUDIO_SAMPLING_FREQUENCY              96000
#define DecimationFactor                     (PDM_Clock_Freq * 1000)/AUDIO_SAMPLING_FREQUENCY
#define AudioInCtxSize                       (PDM_Clock_Freq/8) * 2 * N_MS_PER_INTERRUPT  //768
#define PDM_INTERNAL_BUFFER_SIZE_I2S          ((MAX_MIC_FREQ / 8) * 2 * N_MS_PER_INTERRUPT) //768
#define PDM_BUFFER_SIZE                        ((((AUDIO_CHANNELS * AUDIO_SAMPLING_FREQUENCY) / 1000) * 128) / 16)* N_MS
#define PCM_BUFFER_SIZE                        ((AUDIO_CHANNELS*AUDIO_SAMPLING_FREQUENCY)/1000) * N_MS

#define output_samples_no               (AUDIO_SAMPLING_FREQUENCY/1000) * N_MS_PER_INTERRUPT
#define get_8Bits(reg , pin)             ((reg >> pin*8) & 0x00FF)
#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                   (((uint16_t)(A) & 0x00ff) << 8))

static uint16_t I2S_InternalBuffer[size_i2s_buffer];
uint16_t PDM_Buffer[size_pdm_buffer];
uint16_t PCM_Buffer[size_pcm_buffer];


static volatile uint16_t *  inBuffer;


void SystemClock_Config(void);
void AudioProcess(void)
{
	uint8_t i;
	PDM_Filter( &((uint8_t*)(PDM_Buffer))[0], &PCM_Buffer[0], (PDM_Filter_Handler_t *)&PDM1_filter_handler);
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);

	for(i=0; i<size_pcm_buffer; i++)
	{
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,PCM_Buffer[i]);

	}
	//HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)pcm_buffer,size_pcm_buffer,DAC_ALIGN_12B_R);
}
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	uint32_t index = 0;

	uint16_t * DataTempI2S = I2S_InternalBuffer;
	for(index = 0; index < size_i2s_buffer /2; index++)
	{
		PDM_Buffer[index] = HTONS(DataTempI2S[index]);
	}
	AudioProcess();
}
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{

	uint32_t index = 0;
	uint16_t * DataTempI2S = &I2S_InternalBuffer[size_i2s_buffer /2] ;
	for(index = 0; index <  size_i2s_buffer /2; index++)
	{
		PDM_Buffer[index] = HTONS(DataTempI2S[index]);
	}
	AudioProcess();

}
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2S2_Init();
	MX_CRC_Init();
	MX_PDM2PCM_Init();
	MX_DAC_Init();
	HAL_DAC_MspInit(&hdac);
	//__HAL_I2S_DISABLE(&hi2s2);
	HAL_I2S_MspInit(&hi2s2);
	__HAL_RCC_CRC_CLK_ENABLE();
	CRC->CR = CRC_CR_RESET;
	PDM_Filter_Handler_t PDM1_filter_handler;
	PDM_Filter_Config_t PDM1_filter_config;
	PDM1_filter_handler.bit_order = PDM_FILTER_BIT_ORDER_LSB;
	PDM1_filter_handler.endianness = PDM_FILTER_ENDIANNESS_LE;
	PDM1_filter_handler.high_pass_tap = 2122358088;//2122358088
	PDM1_filter_handler.out_ptr_channels = 1;
	PDM1_filter_handler.in_ptr_channels = 1;
	PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM1_filter_handler));
	PDM1_filter_config.output_samples_number = size_pcm_buffer;
	PDM1_filter_config.mic_gain = 24 ;
	PDM1_filter_config.decimation_factor = PDM_FILTER_DEC_FACTOR_64;
	PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM1_filter_handler,&PDM1_filter_config);


	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_I2S_Receive_DMA((I2S_HandleTypeDef *)&hi2s2,I2S_InternalBuffer,size_i2s_buffer);
	//HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)pcm_buffer,size_pcm_buffer,DAC_ALIGN_12B_R);
	while (1)
	{
		//HAL_I2S_Receive((I2S_HandleTypeDef *)&hi2s2, &i2s_buffer[0], size_i2s_buffer, 10000);


	}

}
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
  PeriphClkInitStruct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLR;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
