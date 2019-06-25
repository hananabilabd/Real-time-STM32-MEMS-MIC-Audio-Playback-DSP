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
#define order 9
#define BLOCK_SIZE 10

const q31_t firCoeff[10] = {
      19595652,    64495878,   196426734,   374937731,   504009917,   504009917,
     374937731,   196426734,    64495878,    19595652
};
static q31_t firState[BLOCK_SIZE + order - 1];


q31_t * Out_Buffer[10];
arm_fir_instance_f32 S;
void BandpassFilter_Init(void){

		  /* Call FIR init function to initialize the instance structure. */
		  arm_fir_init_q31(&S, (uint16_t)order, &firCoeff, &firState,(uint32_t)BLOCK_SIZE );
}
void BandpassFilter(q31_t * inputBuffer,q31_t * outBuffer){
	static int power =10;
	    arm_fir_q31(&S, &inputBuffer[0] , &Out_Buffer[0] , BLOCK_SIZE);


	       // Convert output back to uint16 for plotting
	       for (int i = 0; i < BLOCK_SIZE; i++)
	       {
	           inputBuffer[i] = (uint16_t)(Out_Buffer[i] - 63500);
	       }

}
