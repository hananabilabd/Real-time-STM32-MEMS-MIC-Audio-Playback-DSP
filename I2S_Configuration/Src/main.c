#include "main.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "i2s.h"
#include "pdm2pcm.h"
#include "gpio.h"

#define get_8Bits(reg , pin)             ((reg >> pin*8) & 0x00FF)
#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | \
                   (((uint16_t)(A) & 0x00ff) << 8))

void SystemClock_Config(void);

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

	uint8_t size_pdm_buffer = 64;
	uint8_t size_pcm_buffer = 16;
	uint8_t size_i2s_buffer = size_pdm_buffer/2;//=32
	uint16_t i2s_buffer[size_i2s_buffer];
	uint8_t pdm_buffer[size_pdm_buffer];
	uint16_t pcm_buffer[size_pcm_buffer];

	/*Enables and resets CRC-32 from STM32 HW */
	__HAL_RCC_CRC_CLK_ENABLE();
	CRC->CR = CRC_CR_RESET;
	PDM_Filter_Handler_t PDM1_filter_handler;
	PDM_Filter_Config_t PDM1_filter_config;
	PDM1_filter_handler.bit_order = PDM_FILTER_BIT_ORDER_LSB;
	PDM1_filter_handler.endianness = PDM_FILTER_ENDIANNESS_LE;
	PDM1_filter_handler.high_pass_tap = 2122358088;
	PDM1_filter_handler.out_ptr_channels = 1;
	PDM1_filter_handler.in_ptr_channels = 1;
	PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM1_filter_handler));
	PDM1_filter_config.output_samples_number = 16;
	PDM1_filter_config.mic_gain = 2 ;
	PDM1_filter_config.decimation_factor = PDM_FILTER_DEC_FACTOR_64;
	PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM1_filter_handler,&PDM1_filter_config);
	uint16_t i;
	uint16_t temp ;
	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);


	while (1)
	{

		HAL_I2S_Receive((I2S_HandleTypeDef *)&hi2s2, &i2s_buffer[0], size_i2s_buffer, 10000);
		for(i=0; i<size_i2s_buffer; i++)
		{
			//pdm_buffer[i]=HTONS(i2s_buffer[i]);

			pdm_buffer[i*2] = get_8Bits(i2s_buffer[i],0);
			pdm_buffer[(i*2)+1] = get_8Bits(i2s_buffer[i],1);
		}

		PDM_Filter( &((uint8_t*)(pdm_buffer))[0], &pcm_buffer[0], (PDM_Filter_Handler_t *)&PDM1_filter_handler);
		for(i=0; i<size_pcm_buffer; i++)
		{
			HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,pcm_buffer[i]);
		}
		//HAL_I2S_Receive_DMA(&hAudioInI2s, I2S_InternalBuffer, 160);

	}

}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};


  __HAL_RCC_PLL_PLLM_CONFIG(4);
  /**Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV2;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 10;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 5;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
  PeriphClkInitStruct.PLLI2SDivQ = 1;
  PeriphClkInitStruct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}


void Error_Handler(void)
{

}


