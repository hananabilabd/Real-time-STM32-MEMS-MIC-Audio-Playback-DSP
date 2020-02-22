/*
 * codec_WM8731.h
 *
 *  Created on: Feb 19, 2020
 *      Author: Hanna Nabil
 */

#ifndef INC_CODEC_WM8731_H_
#define INC_CODEC_WM8731_H_

#include "stm32f4xx.h"





/*----------------------------------------------------------------------------*/

/* High Layer codec functions */
uint32_t Codec_Init(uint32_t AudioFreq);

void     Codec_Reset(void);
uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue);


#endif /* INC_CODEC_WM8731_H_ */
