/*
 * parametricEqualizer.c
 *
 *  Created on: Jun 25, 2019
 *      Author: Hanna Nabil
 */
#include"rtwtypes.h"
#include "parametricEqualizer.h"
#include "biquad_coeffs.h"
#include "arm_math.h"
#define buffer_size                10U
typedef struct {
  arm_biquad_cascade_df2T_instance_f32 S;/* synthesized block */
  real32_T Normalizingtheaudiosignalbetwee[buffer_size];/* '<Root>/Normalizing the audio signal between -1.0 to 1.0' */
  real32_T BiquadFilter1[buffer_size];
  real32_T MatrixConcatenate[9];       /* '<S2>/Matrix Concatenate' */
  real32_T MatrixConcatenate1[6];      /* '<S2>/Matrix Concatenate1' */
  real32_T DiscreteFilter[5];          /* '<S6>/Discrete Filter' */
  real32_T DiscreteFilter_g[5];        /* '<S5>/Discrete Filter' */
  real32_T DiscreteFilter_n[5];        /* '<S4>/Discrete Filter' */
  real32_T DiscreteFilter_states[5];   /* '<S6>/Discrete Filter' */
  real32_T DiscreteFilter_states_p[5]; /* '<S5>/Discrete Filter' */
  real32_T DiscreteFilter_states_l[5]; /* '<S4>/Discrete Filter' */
  real32_T coefficients[15];           /* synthesized block */
  real32_T pState[12];                 /* synthesized block */
  real32_T fv0[15];
  int16_T *MicIn;                  /* '<Root>/Mic In' */
  int16_T * iv0;
} DW;

DW rtDW;
extern int gainDB[3];
static void BiquadFilter_Init(void)
{
  int32_T i;

  BiquadFilter1_coefficients(rtDW.MatrixConcatenate, rtDW.MatrixConcatenate1,rtDW.fv0);
  for (i = 0; i < 15; i++) {
    rtDW.coefficients[i] = rtDW.fv0[i];
  }

  arm_biquad_cascade_df2T_init_f32(&rtDW.S, 3U, &rtDW.coefficients[0], &rtDW.pState[0]);
}
void step(void)
{
  int32_T i;
  real32_T tmp;

  /* Gain: '<Root>/Normalizing the audio signal between -1.0 to 1.0' */
  for (i = 0; i < buffer_size; i++) {
    rtDW.Normalizingtheaudiosignalbetwee[i] = 3.05175781E-5F * (real32_T)rtDW.MicIn[i];
  }

  /* End of Gain: '<Root>/Normalizing the audio signal between -1.0 to 1.0' */

  /* Outputs for Atomic SubSystem: '<Root>/Equalizer' */
  /* Outputs for Atomic SubSystem: '<S1>/Bands' */
  Bands();

  /* End of Outputs for SubSystem: '<S1>/Bands' */

  /* Outputs for Atomic SubSystem: '<S1>/BiquadFilter' */
  BiquadFilter();

  /* End of Outputs for SubSystem: '<S1>/BiquadFilter' */
  /* End of Outputs for SubSystem: '<Root>/Equalizer' */

  /* Gain: '<Root>/Denormalizing the audio signal to int16 range' */
  for (i = 0; i < buffer_size; i++) {
    tmp = (real32_T)floor(32768.0F * rtDW.BiquadFilter1[i]);
    if (tmp < 32768.0F) {
      if (tmp >= -32768.0F) {
        rtDW.iv0[i] = (int16_T)tmp;
      } else {
        rtDW.iv0[i] = MIN_int16_T;
      }
    } else {
      rtDW.iv0[i] = MAX_int16_T;
    }
  }

  /* End of Gain: '<Root>/Denormalizing the audio signal to int16 range' */


}
static void BiquadFilter(void)
{
  /* S-Function (sdspbiquad): '<S3>/Biquad Filter 1' */
  BiquadFilter1_coefficients(rtDW.MatrixConcatenate, rtDW.MatrixConcatenate1, rtDW.coefficients);
  arm_biquad_cascade_df2T_f32(&rtDW.S, &rtDW.Normalizingtheaudiosignalbetwee[0],&rtDW.BiquadFilter1[0], buffer_size);
}
static void BiquadFilter1_coefficients(const real32_T u2[9], const real32_T u3[6],
  real32_T y[15])
{
  real32_T denumValues[6];
  int32_T i;
  int32_T denumValues_tmp;

  /* Update for S-Function (sdspbiquad): '<S3>/Biquad Filter 1' */
  for (i = 0; i < 6; i++) {
    denumValues[i] = u3[i];
  }

  for (i = 0; i < 3; i++) {
    denumValues_tmp = i << 1;
    denumValues[denumValues_tmp] = -denumValues[denumValues_tmp];
    denumValues[1 + denumValues_tmp] = -denumValues[denumValues_tmp + 1];
  }

  for (i = 0; i < 3; i++) {
    y[5 * i] = u2[3 * i];
    y[1 + 5 * i] = u2[3 * i + 1];
    y[2 + 5 * i] = u2[3 * i + 2];
    y[3 + 5 * i] = denumValues[i << 1];
    y[4 + 5 * i] = denumValues[(i << 1) + 1];
  }

  /* End of Update for S-Function (sdspbiquad): '<S3>/Biquad Filter 1' */
}
static void Bands(void)
{
  /* Outputs for Atomic SubSystem: '<S2>/Band 1' */
  Band1();

  /* End of Outputs for SubSystem: '<S2>/Band 1' */

  /* Outputs for Atomic SubSystem: '<S2>/Band 2' */
  Band2();

  /* End of Outputs for SubSystem: '<S2>/Band 2' */

  /* Outputs for Atomic SubSystem: '<S2>/Band 3' */
  Band3();

  /* End of Outputs for SubSystem: '<S2>/Band 3' */

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In1' */
  rtDW.MatrixConcatenate1[0] = rtDW.DiscreteFilter_n[3];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In2' */
  rtDW.MatrixConcatenate1[2] = rtDW.DiscreteFilter_g[3];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In3' */
  rtDW.MatrixConcatenate1[4] = rtDW.DiscreteFilter[3];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In1' */
  rtDW.MatrixConcatenate1[1] = rtDW.DiscreteFilter_n[4];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In2' */
  rtDW.MatrixConcatenate1[3] = rtDW.DiscreteFilter_g[4];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix Concatenate1In3' */
  rtDW.MatrixConcatenate1[5] = rtDW.DiscreteFilter[4];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn1' */
  rtDW.MatrixConcatenate[0] = rtDW.DiscreteFilter_n[0];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn2' */
  rtDW.MatrixConcatenate[3] = rtDW.DiscreteFilter_g[0];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn3' */
  rtDW.MatrixConcatenate[6] = rtDW.DiscreteFilter[0];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn1' */
  rtDW.MatrixConcatenate[1] = rtDW.DiscreteFilter_n[1];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn2' */
  rtDW.MatrixConcatenate[4] = rtDW.DiscreteFilter_g[1];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn3' */
  rtDW.MatrixConcatenate[7] = rtDW.DiscreteFilter[1];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn1' */
  rtDW.MatrixConcatenate[2] = rtDW.DiscreteFilter_n[2];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn2' */
  rtDW.MatrixConcatenate[5] = rtDW.DiscreteFilter_g[2];

  /* SignalConversion: '<S2>/ConcatBufferAtMatrix ConcatenateIn3' */
  rtDW.MatrixConcatenate[8] = rtDW.DiscreteFilter[2];
}
static void Band3(void)
{
  real32_T denAccum;
  int32_T k;
  for (k = 0; k < 5; k++) {
    /* DiscreteFilter: '<S6>/Discrete Filter' incorporates:
     *  Constant: '<S1>/Coeffs for Band3'
     */
    denAccum = CoeffsMatrix3[gainDB[2]][k] - -0.75F * rtDW.DiscreteFilter_states[k];
    rtDW.DiscreteFilter[k] = 0.25F * denAccum;

    /* Update for DiscreteFilter: '<S6>/Discrete Filter' */
    rtDW.DiscreteFilter_states[k] = denAccum;
  }
}
static void Band1(void)
{
  real32_T denAccum;
  int32_T k;
  for (k = 0; k < 5; k++) {
    /* DiscreteFilter: '<S4>/Discrete Filter' incorporates:
     *  Constant: '<S1>/Coeffs for Band1'
     */
    denAccum = CoeffsMatrix1[gainDB[0]][k] - -0.75F * rtDW.DiscreteFilter_states_l[k];
    rtDW.DiscreteFilter_n[k] = 0.25F * denAccum;

    /* Update for DiscreteFilter: '<S4>/Discrete Filter' */
    rtDW.DiscreteFilter_states_l[k] = denAccum;
  }
}

/* Output and update for atomic system: '<S2>/Band 2' */
static void Band2(void)
{
  real32_T denAccum;
  int32_T k;
  for (k = 0; k < 5; k++) {
    /* DiscreteFilter: '<S5>/Discrete Filter' incorporates:
     *  Constant: '<S1>/Coeffs for Band2'
     */
    denAccum = CoeffsMatrix2[gainDB[1]][k] - -0.75F * rtDW.DiscreteFilter_states_p[k];
    rtDW.DiscreteFilter_g[k] = 0.25F * denAccum;

    /* Update for DiscreteFilter: '<S5>/Discrete Filter' */
    rtDW.DiscreteFilter_states_p[k] = denAccum;
  }
}
void parametricEqualizer_Init(int16_T *Input ,int16_T *Output){
	BiquadFilter_Init();
	rtDW.MicIn = Input;
	rtDW.iv0 = Output;
}
