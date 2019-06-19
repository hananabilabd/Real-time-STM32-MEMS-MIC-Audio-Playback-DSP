/*
typedef struct{
  uint16_t bit_order;
  uint16_t endianness;
  uint32_t high_pass_tap;
  uint16_t in_ptr_channels;
  uint16_t out_ptr_channels;
  uint32_t pInternalMemory[PDM2PCM_INTERNAL_MEMORY_SIZE];
}PDM_Filter_Handler_t;
typedef struct{
  uint16_t decimation_factor;
  uint16_t output_samples_number;
  int16_t  mic_gain;
}PDM_Filter_Config_t;
typedef struct {
	int32_t Z;
	int32_t oldOut;
	int32_t oldIn;
}HP_FilterState_TypeDef;
typedef struct
{
  uint32_t                    Instance;
  uint32_t                    Device;
  uint32_t                    SampleRate;
  uint32_t                    BitsPerSample;
  uint32_t                    ChannelsNbr;
  uint16_t                    *pBuff;
  uint8_t                     **pMultiBuff;
  uint32_t                    Size;
  uint32_t                    Volume;
  uint32_t                    State;
  uint32_t                    IsMultiBuff;
  uint32_t                    IsMspCallbacksValid;
  HP_FilterState_TypeDef 	  HP_Filters[4];
  uint32_t DecimationFactor;
}AUDIO_IN_Ctx_t;
typedef struct
{
  uint32_t                    Device;
  uint32_t                    SampleRate;
  uint32_t                    BitsPerSample;
  uint32_t                    ChannelsNbr;
  uint32_t                    Volume;
}BSP_AUDIO_Init_t;
static PDM_Filter_Handler_t  PDM_FilterHandler[4];
static PDM_Filter_Config_t   PDM_FilterConfig[4];
AUDIO_IN_Ctx_t                         AudioInCtx[3] = {0};
static uint16_t I2S_InternalBuffer[768];
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  uint32_t index = 0;
      uint16_t * DataTempI2S = &I2S_InternalBuffer[80] ;
      for(index = 0; index < 80; index++)
      {
        AudioInCtx[0].pBuff[index] = HTONS(DataTempI2S[index]);
      }
  //BSP_AUDIO_IN_TransferComplete_CallBack(0);
}
void BSP_AUDIO_IN_PDMToPCM_Init(uint32_t Instance, uint32_t AudioFreq, uint32_t ChnlNbrIn, uint32_t ChnlNbrOut)
{
	AudioInCtx[Instance].pBuff = (uint8_t *) pdm_buffer;
    uint32_t index = 0;
    for(index = 0; index < ChnlNbrIn; index++)
    {
      volatile uint32_t error = 0;

      PDM_FilterHandler[index].bit_order  = PDM_FILTER_BIT_ORDER_LSB;
      PDM_FilterHandler[index].endianness = PDM_FILTER_ENDIANNESS_LE;
      PDM_FilterHandler[index].high_pass_tap = 2122358088;
      PDM_FilterHandler[index].out_ptr_channels = ChnlNbrOut;
      PDM_FilterHandler[index].in_ptr_channels  = ChnlNbrIn;

      PDM_FilterConfig[index].output_samples_number = (AudioFreq/1000) ;
      PDM_FilterConfig[index].mic_gain = 24;

      switch (AudioInCtx[0].DecimationFactor)
      {
      case 16:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_16;
        break;
      case 24:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_24;
        break;
      case 32:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_32;
        break;
      case 48:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_48;
        break;
      case 64:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_64;
        break;
      case 80:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        break;
      case 128:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_128;
        break;
      case 160:
        PDM_FilterConfig[index].decimation_factor = PDM_FILTER_DEC_FACTOR_80;
        PDM_FilterConfig[index].output_samples_number *= 2;
        PDM_FilterHandler[index].out_ptr_channels = 1;
        break;

      }

      PDM_Filter_Init((PDM_Filter_Handler_t *)(&PDM_FilterHandler[index]));
      PDM_Filter_setConfig((PDM_Filter_Handler_t *)&PDM_FilterHandler[index], &PDM_FilterConfig[index]);

    }
}
void BSP_AUDIO_IN_PDMToPCM(uint32_t Instance, uint16_t *PDMBuf, uint16_t *PCMBuf)
{
	uint8_t index =0;
    PDM_Filter(&((uint8_t*)(PDMBuf))[index], (uint16_t*)&(PCMBuf[index]), &PDM_FilterHandler[index]);
}
*/
