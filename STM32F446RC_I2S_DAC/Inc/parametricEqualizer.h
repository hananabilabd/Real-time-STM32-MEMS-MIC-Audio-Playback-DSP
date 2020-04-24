/*
 * parametricEqualizer.h
 *
 *  Created on: Jun 25, 2019
 *      Author: Hanna Nabil
 */

#ifndef PARAMETRICEQUALIZER_H_
#define PARAMETRICEQUALIZER_H_

static void Band1(void);
static void Band2(void);
static void Band3(void);
static void Bands(void);
static void BiquadFilter_Init(void);
static void BiquadFilter(void);
static void BiquadFilter1_coefficients(const real32_T u2[9], const real32_T u3[6],real32_T y[15]);
void parametricEqualizer_Init(int16_T *Input ,int16_T *Output);
void step(void);

#endif /* PARAMETRICEQUALIZER_H_ */
