/*
 * bandpass.c
 *
 *  Created on: Jun 23, 2019
 *      Author: Hanna Nabil
 */



#include "arm_math.h"
#include "math_helper.h"
#include "cascade_example.h"
#include "bandpass.h"
#include "rtwtypes.h"
#define order 9
#define BLOCK_SIZE 10

const float32_t firCoeff[9] = {
		0.0020641473f,  0.0262557654f,   0.119240402f,    0.25464889f,
		     0.321469395f,    0.25464889f,   0.119240402f,  0.0262557654f,
		    0.0020641473f
};


static float32_t firState[BLOCK_SIZE + order - 1];

float32_t  InBuffer[10];
float32_t  OutBuffer[10];
int16_t *MicInput;
int16_t *DACOutput;
arm_fir_instance_f32 S;
void BandpassFilter_Init(int16_t * inputBuffer,int16_t * outBuffer){

		  /* Call FIR init function to initialize the instance structure. */
		  arm_fir_init_f32(&S, (uint16_t)order, &firCoeff[0], &firState[0],(uint32_t)BLOCK_SIZE );
		  MicInput =inputBuffer;
		  DACOutput =outBuffer;
}
void BandpassFilter(void){
	real32_T tmp;
	 for (int i = 0; i < BLOCK_SIZE; i++) {
	    InBuffer[i] = 3.05175781E-5F * (real32_T)MicInput[i];
	  }
	    arm_fir_f32(&S, &InBuffer[0] , &OutBuffer[0] , BLOCK_SIZE);
	    for (int i = 0; i < BLOCK_SIZE; i++) {
	  	    tmp = (real32_T)floor(32768.0F * OutBuffer[i]);
	  	    if (tmp < 32768.0F) {
	  	      if (tmp >= -32768.0F) {
	  	        DACOutput[i] = (int16_T)tmp;
	  	      } else {
	  	    	DACOutput[i] = MIN_int16_T;
	  	      }
	  	    } else {
	  	    	DACOutput[i] = MAX_int16_T;
	  	    }
	  	  }



}
