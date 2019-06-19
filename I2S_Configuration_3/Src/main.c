/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "pdm2pcm.h"
#include "gpio.h"
#include "MY_CS43L22.h"
#include "arm_math.h"
#include "math_helper.h"
#include "cascade_example.h"


#define get_8Bits(reg , pin)             ((reg >> pin*8) & 0x00FF)
#define HTONS(A)  ((((uint16_t)(A) & 0xff00) >> 8) | (((uint16_t)(A) & 0x00ff) << 8))


#define size_i2s_buffer  32 //=32
#define size_pdm_buffer  200 //64
#define size_pcm_buffer  10  //10
static uint16_t I2S_InternalBuffer[size_i2s_buffer];
uint16_t PDM_Buffer[size_pdm_buffer];
uint16_t PCM_Buffer[size_pcm_buffer];

void cascade(void){
	float32_t  *inputF32, *outputF32;
	  arm_biquad_cas_df1_32x64_ins_q31 S1;
	  arm_biquad_cas_df1_32x64_ins_q31 S2;
	  arm_biquad_casd_df1_inst_q31 S3;
	  arm_biquad_casd_df1_inst_q31 S4;
	  arm_biquad_casd_df1_inst_q31 S5;
	  int i;
	  int32_t status;

	  inputF32 = &testInput_f32[0];
	  outputF32 = &testOutput[0];

	  /* Initialize the state and coefficient buffers for all Biquad sections */

	  arm_biquad_cas_df1_32x64_init_q31(&S1, NUMSTAGES,
	            (q31_t *) &coeffTable[190*0 + 10*(gainDB[0] + 9)],
	            &biquadStateBand1Q31[0], 2);

	  arm_biquad_cas_df1_32x64_init_q31(&S2, NUMSTAGES,
	            (q31_t *) &coeffTable[190*1 + 10*(gainDB[1] + 9)],
	            &biquadStateBand2Q31[0], 2);

	  arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
	          (q31_t *) &coeffTable[190*2 + 10*(gainDB[2] + 9)],
	          &biquadStateBand3Q31[0], 2);

	  arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
	          (q31_t *) &coeffTable[190*3 + 10*(gainDB[3] + 9)],
	          &biquadStateBand4Q31[0], 2);

	  arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
	          (q31_t *) &coeffTable[190*4 + 10*(gainDB[4] + 9)],
	          &biquadStateBand5Q31[0], 2);


	  /* Call the process functions and needs to change filter coefficients
	     for varying the gain of each band */

	  for(i=0; i < NUMBLOCKS; i++)
	  {

	    /* ----------------------------------------------------------------------
	    ** Convert block of input data from float to Q31
	    ** ------------------------------------------------------------------- */

	    arm_float_to_q31(inputF32 + (i*BLOCKSIZE), inputQ31, BLOCKSIZE);

	    /* ----------------------------------------------------------------------
	    ** Scale down by 1/8.  This provides additional headroom so that the
	    ** graphic EQ can apply gain.
	    ** ------------------------------------------------------------------- */

	    arm_scale_q31(inputQ31, 0x7FFFFFFF, -3, inputQ31, BLOCKSIZE);

	    /* ----------------------------------------------------------------------
	    ** Call the Q31 Biquad Cascade DF1 32x64 process function for band1, band2
	    ** ------------------------------------------------------------------- */

	    arm_biquad_cas_df1_32x64_q31(&S1, inputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cas_df1_32x64_q31(&S2, outputQ31, outputQ31, BLOCKSIZE);

	    /* ----------------------------------------------------------------------
	    ** Call the Q31 Biquad Cascade DF1 process function for band3, band4, band5
	    ** ------------------------------------------------------------------- */

	    arm_biquad_cascade_df1_q31(&S3, outputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_q31(&S4, outputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_q31(&S5, outputQ31, outputQ31, BLOCKSIZE);

	    /* ----------------------------------------------------------------------
	    ** Convert Q31 result back to float
	    ** ------------------------------------------------------------------- */

	    arm_q31_to_float(outputQ31, outputF32 + (i * BLOCKSIZE), BLOCKSIZE);

	    /* ----------------------------------------------------------------------
	    ** Scale back up
	    ** ------------------------------------------------------------------- */

	    arm_scale_f32(outputF32 + (i * BLOCKSIZE), 8.0f, outputF32 + (i * BLOCKSIZE), BLOCKSIZE);
	  };
}



void SystemClock_Config(void);
char AudioProcessFlag=0;
void AudioProcess(void)
{
	uint8_t i;
	PDM_Filter( &((uint8_t*)(PDM_Buffer))[0], &PCM_Buffer[0], (PDM_Filter_Handler_t *)&PDM1_filter_handler);

	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	/*for(i=0; i<size_pcm_buffer; i++)
	{
		HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,PCM_Buffer[i]);
	}*/

	// send the PCM_buffer by I2S3 to the audio codec CS43L22
	HAL_I2S_Transmit_DMA((I2S_HandleTypeDef *)&hi2s3, PCM_Buffer, size_pcm_buffer);
	//

	//HAL_DAC_Stop_DMA(&hdac,DAC_CHANNEL_1);
	//HAL_DAC_Start_DMA(&hdac,DAC_CHANNEL_1,(uint32_t *)PCM_Buffer,size_pcm_buffer,DAC_ALIGN_12B_R);
}
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{

	uint32_t index = 0;
	uint16_t * DataTempI2S = I2S_InternalBuffer;
	for(index = 0; index < size_i2s_buffer /2; index++)
	{
		PDM_Buffer[index] = HTONS(DataTempI2S[index]);
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
		PDM_Buffer[index] = HTONS(DataTempI2S[index]);
	}
	AudioProcessFlag = 1;
	//AudioProcess();


}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s3)
{

}
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s3)
{

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

// init the CS43 in I2S mode
  CS43_Init(hi2c1, MODE_I2S);
  CS43_SetVolume(50);
  CS43_Enable_RightLeft(CS43_RIGHT_LEFT);
  CS43_Start();
//
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
  /* USER CODE END 3 */
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
