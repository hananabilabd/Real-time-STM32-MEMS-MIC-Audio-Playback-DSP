/*
 * pdmfilter.c
 *
 *  Created on: Jun 25, 2019
 *      Author: Hanna Nabil
 */

#include <stdio.h>

#ifndef uint16_t
typedef unsigned short int uint16_t;
typedef signed short int int16_t;
typedef unsigned long int uint32_t;
#endif

void pdmFilter(uint16_t *mic_buff,int16_t *MW_pcm_data,uint32_t num_samples)
{
	static uint32_t prev_buffer[4];
	uint32_t i=0,j=0,k=0;
	uint32_t Current_Value=0,Prev_Value=0;
	uint32_t pcm_data=0;

	for(k=0;k<num_samples;k++)
	{
		pcm_data= 0;
		for(j=0;j<4;j++)
		{
			Current_Value = mic_buff[j+(k*4)];
			Prev_Value = prev_buffer[j];
			/*Copy from the Current Data Sample to Previous Data Sample*/
			prev_buffer[j] = Current_Value;
			for(i=0;i<16;i++)
			{
				pcm_data += ((((Prev_Value&0x8000)&&1)*(i+(16*j))) + ((Current_Value&0x8000)&&1)*(64-i-(16*j)));
				Current_Value = Current_Value<<1;
				Prev_Value = Prev_Value<<1;
			}
		}
		/* Now Perform the Shift - Since already in the 4096 range for Order 7 shift only by 4 and so on*/
		MW_pcm_data[k] = (int16_t)((pcm_data<<4) - 32768);
	}
}
