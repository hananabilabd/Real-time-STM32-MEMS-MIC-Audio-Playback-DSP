/*
 * bandpass.h
 *
 *  Created on: Jun 24, 2019
 *      Author: Hanna Nabil
 */

#ifndef BANDPASS_H_
#define BANDPASS_H_

void BandpassFilter_Init(void);
void BandpassFilter(int32_t * inputBuffer,  int32_t* outBuffer);
void arm_power_q31(
  const int32_t * pSrc,
        uint32_t blockSize,
        int64_t * pResult);

#endif /* BANDPASS_H_ */
