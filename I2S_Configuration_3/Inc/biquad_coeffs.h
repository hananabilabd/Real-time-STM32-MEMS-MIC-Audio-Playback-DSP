/*
 * biquad_coeffs.h
 *
 *  Created on: Jun 25, 2019
 *      Author: Hanna Nabil
 */

#ifndef BIQUAD_COEFFS_H_
#define BIQUAD_COEFFS_H_

extern real32_T CoeffsMatrix1[5];      /* Design Specifications
                                          Sampling Frequency : 44100 Hz
                                          Response           : Parametric Equalizer
                                          Specification      : Fc, BW, G
                                          Fc                 : 8.916000e+02 Hz
                                          BW                 : 300 Hz
                                          G                  : 3 dB
                                        */
extern real32_T CoeffsMatrix2[5];      /* Design Specifications
                                          Sampling Frequency : 44100 Hz
                                          Response           : Parametric Equalizer
                                          Specification      : Fc, BW, G
                                          Fc                 : 1 KHz
                                          BW                 : 700 Hz
                                          G                  : -2 dB
                                        */
extern real32_T CoeffsMatrix3[5];      /* Design Specifications
                                          Sampling Frequency : 44100 Hz
                                          Response           : Parametric Equalizer
                                          Specification      : Fc, BW, G
                                          Fc                 : 3 KHz
                                          BW                 : 500 Hz
                                          G                  : 2 dB
                                        */

#endif /* BIQUAD_COEFFS_H_ */
