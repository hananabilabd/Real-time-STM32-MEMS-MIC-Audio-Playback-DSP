/*
 * cascade.c
 *
 *  Created on: Jun 23, 2019
 *      Author: Hanna Nabil
 */

#include "arm_math.h"
#include "math_helper.h"
#include "cascade_example.h"
/*
void cascade(void){



	  inputF32 = &testInput_f32[0];
	  outputF32 = &testOutput[0];


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




	  for(i=0; i < NUMBLOCKS; i++)
	  {



	    arm_float_to_q31(inputF32 + (i*BLOCKSIZE), inputQ31, BLOCKSIZE);



	    arm_scale_q31(inputQ31, 0x7FFFFFFF, -3, inputQ31, BLOCKSIZE);



	    arm_biquad_cas_df1_32x64_q31(&S1, inputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cas_df1_32x64_q31(&S2, outputQ31, outputQ31, BLOCKSIZE);



	    arm_biquad_cascade_df1_q31(&S3, outputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_q31(&S4, outputQ31, outputQ31, BLOCKSIZE);
	    arm_biquad_cascade_df1_q31(&S5, outputQ31, outputQ31, BLOCKSIZE);


	    arm_q31_to_float(outputQ31, outputF32 + (i * BLOCKSIZE), BLOCKSIZE);



	    arm_scale_f32(outputF32 + (i * BLOCKSIZE), 8.0f, outputF32 + (i * BLOCKSIZE), BLOCKSIZE);
	  };
}
*/
