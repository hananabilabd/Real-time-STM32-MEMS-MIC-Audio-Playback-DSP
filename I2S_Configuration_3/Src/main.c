
#include "main.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "pdm2pcm.h"
#include "gpio.h"
#include "MY_CS43L22.h"
//#include "arm_math.h"
//#include "math_helper.h"
//#include "cascade_example.h"
#include "Equalizer.h"
#include "bandpass.h"
#include "rtwtypes.h"
#include "parametricEqualizer.h"
void pdmFilter(uint16_t *mic_buff,int16_t *MW_pcm_data,uint32_t num_samples);
#define get_8Bits(reg , pin)             ((reg >> pin*8) & 0x00FF)
#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))
#define MW_MIC_FRAME_LENGTH   882
#define MW_PDM_SAMPLES_UINT16_INPUT_BUFFER_SIZE  ((MW_MIC_FRAME_LENGTH) * (64) * (1) / 8 / 2)

#define size_i2s_buffer  MW_PDM_SAMPLES_UINT16_INPUT_BUFFER_SIZE *2//=32
#define size_pdm_buffer  MW_PDM_SAMPLES_UINT16_INPUT_BUFFER_SIZE //64
#define size_pcm_buffer  882  //10
static uint16_t I2S_InternalBuffer[size_i2s_buffer];
uint16_t PDM_Buffer[size_pdm_buffer];
int16_t PCM_Buffer[size_pcm_buffer];

int16_t Out_Buffer[size_pcm_buffer];


void stm32f4_mic_i2s_init(uint32_t sample_freq)
{
    uint8_t i2s_div, i2s_odd;
    I2S_InitTypeDef I2S_InitType;

    /* Enable clock to SPI2 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* I2S structure initialization */
    I2S_InitType.I2S_AudioFreq = sample_freq;
    I2S_InitType.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    I2S_InitType.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitType.I2S_Mode = I2S_Mode_MasterRx;
    I2S_InitType.I2S_Standard = I2S_Standard_LSB;
    I2S_InitType.I2S_CPOL = I2S_CPOL_High;

    /* Calculate I2S prescalers */
    /* The sampling frequency is increased by MW_PDM_DECIMATION_FACTOR as mic produces
       audio samples in PDM format */
    MW_I2S_GetPrescaler(sample_freq * MW_PDM_DECIMATION_FACTOR, /* uint32_t audio_sample_freqHz, */
            1, /* uint8_t num_chls, */
            1, /* uint8_t nb_bits_per_sample, */
            I2S_InitType.I2S_MCLKOutput, /* uint8_t enable_master_clk, */
            &i2s_div,
            &i2s_odd);

    /* Set the I2S Prescalers */
    MW_I2S_SetPrescaler(SPI2, i2s_div, i2s_odd, &I2S_InitType);

    /* Enable DMA for Rx */
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

    /* Enable I2S */
    I2S_Cmd(SPI2, ENABLE);
}
void SystemClock_Config(void);
char AudioProcessFlag=0;
void AudioProcess(void)
{
	uint8_t i;
	//PDM_Filter( &((uint8_t*)(PDM_Buffer))[0], &PCM_Buffer[0], (PDM_Filter_Handler_t *)&PDM1_filter_handler);
	pdmFilter(PDM_Buffer,PCM_Buffer,MW_MIC_FRAME_LENGTH);
	//step();
	//BandpassFilter(&PCM_Buffer[0] ,&Out_Buffer[0]);
	//equaizerProcess(&PCM_Buffer[0],&Out_Buffer[0]);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	for(i=0; i<size_pcm_buffer; i++)
	{
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,PCM_Buffer[i]);
	}

	// send the PCM_buffer by I2S3 to the audio codec CS43L22
	//HAL_I2S_Transmit_DMA((I2S_HandleTypeDef *)&hi2s3, PCM_Buffer, size_pcm_buffer);


	//HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_1);
	//HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)PCM_Buffer,size_pcm_buffer,DAC_ALIGN_12B_R);
}
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{

	uint32_t index = 0;
	uint16_t * DataTempI2S = I2S_InternalBuffer;
	for(index = 0; index < size_i2s_buffer /2; index++)
	{
		//PDM_Buffer[index] = HTONS(DataTempI2S[index]);
		PDM_Buffer[index] = DataTempI2S[index];
	}
	AudioProcessFlag = 1;
	//AudioProcess();

}
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	uint32_t index = 0;
	uint16_t * DataTempI2S = &I2S_InternalBuffer[size_i2s_buffer /2] ;
	for(index = 0; index <  size_i2s_buffer /2; index++)
	{
		//PDM_Buffer[index] = HTONS(DataTempI2S[index]);
		PDM_Buffer[index] = DataTempI2S[index];
	}
	AudioProcessFlag = 1;
	//AudioProcess();
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
  MX_I2S3_Init();
  MX_I2C1_Init();
/*
// init the CS43 in I2S mode
  CS43_Init(hi2c1, MODE_I2S);
  CS43_SetVolume(50);
  CS43_Enable_RightLeft(CS43_RIGHT_LEFT);
  CS43_Start();
*/
  HAL_DAC_MspInit(&hdac);
  	//__HAL_I2S_DISABLE(&hi2s2);
  	HAL_I2S_MspInit(&hi2s2);
  	__HAL_RCC_CRC_CLK_ENABLE();
  	CRC->CR = CRC_CR_RESET;
  	PDM_Filter_Handler_t PDM1_filter_handler;
  	PDM_Filter_Config_t PDM1_filter_config;
  	PDM1_filter_handler.bit_order = PDM_FILTER_BIT_ORDER_LSB;
  	PDM1_filter_handler.endianness = PDM_FILTER_ENDIANNESS_LE;
  	PDM1_filter_handler.high_pass_tap = 2122358088;//2122358088 ,2104533974
  	PDM1_filter_handler.out_ptr_channels = 1;
  	PDM1_filter_handler.in_ptr_channels = 1;
  	PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM1_filter_handler));
  	PDM1_filter_config.output_samples_number = size_pcm_buffer;
  	PDM1_filter_config.mic_gain = 24 ;
  	PDM1_filter_config.decimation_factor = PDM_FILTER_DEC_FACTOR_64;
  	PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM1_filter_handler,&PDM1_filter_config);
  	//Equalizer_Init(&PCM_Buffer[0],&Out_Buffer[0]);
  	//SVC_init();
  	parametricEqualizer_Init(&PCM_Buffer[0],&Out_Buffer[0]);
  	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
  	HAL_I2S_Receive_DMA((I2S_HandleTypeDef *)&hi2s2,I2S_InternalBuffer,size_i2s_buffer);
  	//HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)PCM_Buffer,size_pcm_buffer,DAC_ALIGN_12B_R);

  	//cascade();

  while (1)
  {

	  if ( AudioProcessFlag ==1){
	  			AudioProcess();
	  			AudioProcessFlag =0;
	  		}


  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
  PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV2;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 5;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
  PeriphClkInitStruct.PLLI2SDivQ = 1;
  PeriphClkInitStruct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
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
