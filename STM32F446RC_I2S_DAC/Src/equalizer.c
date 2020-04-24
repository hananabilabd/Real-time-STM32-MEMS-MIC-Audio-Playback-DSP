/*
 * equalizer.c
 *
 *  Created on: Jun 23, 2019
 *      Author: Hanna Nabil
 */

#include "main.h"
#include "audio_fw_glo.h"
#include "greq_glo.h"
#include "Equalizer.h"
#include <stdint.h>
#include <stdlib.h>
#include "stm32f4xx_hal_gpio.h"
#include "svc_glo.h"
#include "rtwtypes.h"

#define NULL ((void *)0)
#define bufferSize  10
/* Graphic Equalizer */
void *pGreqPersistentMem = NULL;
void *pGreqScratchMem = NULL;
static  greq_static_param_t greq_static_param;
static  greq_dynamic_param_t greq_dynamic_param;
static  buffer_t EqualizerIn;
static  buffer_t EqualizerOut;
static  buffer_t SVC_In;
static  buffer_t SVC_Out;

static real32_T Normalizingtheaudiosignalbetwee[bufferSize];
static real32_T Equalized[bufferSize];

void *pSvcPersistentMem = NULL;
void *pSvcScratchMem = NULL;

static  svc_static_param_t svc_static_param;
static  svc_dynamic_param_t svc_dynamic_param;


static int16_t UserGaindB[GREQ_NB_BANDS_10] = {-12,-12,-12,-12,-3,12,12,12,12,-3};    /* Gain for each band*/
void SVC_init(){
	int32_t error = GREQ_ERROR_NONE;

	pSvcPersistentMem = malloc(svc_persistent_mem_size); /* svc_persistent_mem_size  0x8BC */
	  pSvcScratchMem = malloc(svc_scratch_mem_size);       /* svc_scratch_mem_size  0x1680 */
	  error = svc_reset(pSvcPersistentMem, pSvcScratchMem);
	    if (error != SVC_ERROR_NONE)
	    {
	    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	    }

	        svc_static_param.delay_len = 0;
	        svc_static_param.joint_stereo = 1;

	        error = svc_setParam(&svc_static_param, pSvcPersistentMem);
	        if (error != SVC_ERROR_NONE)
	        {
	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	        }
	        /* SVC dynamic parameters that can be updated here every frame if required */
	        svc_dynamic_param.mute = 0;             /* disable mute */
	        svc_dynamic_param.target_volume_dB = 1; /* volume targeted value */

	        svc_dynamic_param.enable_compr = 1;     /* enable compression */
	        svc_dynamic_param.attack_time = 2103207220;
	        svc_dynamic_param.release_time = 2146924480;
	        svc_dynamic_param.quality = 1;	        /* High quality */

	        error = svc_setConfig(&svc_dynamic_param, pSvcPersistentMem);
	        if (error != SVC_ERROR_NONE)
	        {
	        	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	        }

	        SVC_In.nb_bytes_per_Sample =SVC_Out.nb_bytes_per_Sample =2; /* 8 bits in 0ne byte */
	        SVC_In.nb_channels =SVC_Out.nb_channels = 2; /* stereo */
	        SVC_In.buffer_size =SVC_Out.buffer_size =5; /* just half buffer is process (size per channel) */
	        SVC_In.mode = SVC_Out.mode =INTERLEAVED;
}
void Equalizer_Init(uint16_t PCM_Buffer[],uint16_t Out_Buffer[])
{
	int32_t error = GREQ_ERROR_NONE;
	/* Allocat mem for GrEq */
	pGreqPersistentMem = malloc(  greq_persistent_mem_size); /* greq_persistent_mem_size  0x224 */
	pGreqScratchMem = malloc(greq_scratch_mem_size);       /* greq_scratch_mem_size  0xF00 */

	/* GREQ effect reset */
	error = greq_reset(pGreqPersistentMem, pGreqScratchMem);
	if (error != GREQ_ERROR_NONE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	/* GREQ effect static parameters setting */
	greq_static_param.nb_bands = GREQ_NB_BANDS_10;  /* 10-bands equalizer */
	error = greq_setParam(&greq_static_param, pGreqPersistentMem);
	if (error != GREQ_ERROR_NONE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	/* GREQ dynamic parameters that can be updated here every frame if required */
	greq_dynamic_param.enable = 1;            /* Enables the effect */
	for ( int i =0 ; i<GREQ_NB_BANDS_10 ;i++ ){
		greq_dynamic_param.user_gain_per_band_dB[i] = UserGaindB[i]; /* Gain for band 1 */
	}
	greq_dynamic_param.gain_preset_idx = GREQ_NO_PRESET;         /* User preset defined above */
	error = greq_setConfig(&greq_dynamic_param, pGreqPersistentMem);
	if (error != GREQ_ERROR_NONE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
	/* I/O buffers settings */
	EqualizerIn.nb_bytes_per_Sample      =EqualizerOut.nb_bytes_per_Sample      =  2;
	EqualizerIn.buffer_size              =EqualizerOut.buffer_size              =   10; /* just half buffer is process (size per channel) */
	EqualizerIn.mode                     =EqualizerOut.mode                     =  INTERLEAVED;
	EqualizerIn.nb_channels              = 2;
	EqualizerOut.nb_channels              =  1;
	//EqualizerIn.data_ptr                 =EqualizerOut.data_ptr                 =  &PCM_Buffer[0];

}
void equaizerProcess(uint16_t In_Buffer[],uint16_t Out_Buffer[]){
	real32_T tmp;
	int32_t error = GREQ_ERROR_NONE;
	 for (int i = 0; i < bufferSize; i++) {
	    Normalizingtheaudiosignalbetwee[i] = 3.05175781E-5F * (real32_T)In_Buffer[i];
	  }
	EqualizerIn.data_ptr = &Normalizingtheaudiosignalbetwee[0];
	EqualizerOut.data_ptr = &Equalized[0];
	/*error = svc_process(&SVC_In, &SVC_In, pSvcPersistentMem);
	if (error != SVC_ERROR_NONE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}*/
	error = greq_process(&EqualizerIn,&EqualizerOut, pGreqPersistentMem);
	  for (int i = 0; i < bufferSize; i++) {
	    tmp = (real32_T)floor(32768.0F * Equalized[i]);
	    if (tmp < 32768.0F) {
	      if (tmp >= -32768.0F) {
	        Out_Buffer[i] = (int16_T)tmp;
	      } else {
	        Out_Buffer[i] = MIN_int16_T;
	      }
	    } else {
	      Out_Buffer[i] = MAX_int16_T;
	    }
	  }
	if (error != GREQ_ERROR_NONE)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}

}
