/*
 * AudioExampleLaws.h
 *
 *  Created on: Jun 15, 2019
 *      Author: Hanna Nabil
 */

#ifndef AUDIOEXAMPLELAWS_H_
#define AUDIOEXAMPLELAWS_H_

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
#define output_samples_no               	(AUDIO_SAMPLING_FREQUENCY/1000) * N_MS_PER_INTERRUPT
#endif /* AUDIOEXAMPLELAWS_H_ */
