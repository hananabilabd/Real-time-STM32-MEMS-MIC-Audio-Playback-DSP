/*
 * Equalizer.h
 *
 *  Created on: Jun 23, 2019
 *      Author: Hanna Nabil
 */

#ifndef EQUALIZER_H_
#define EQUALIZER_H_
void Equalizer_Init(uint16_t In_Buffer[],uint16_t Out_Buffer[]);
void equaizerProcess(uint16_t In_Buffer[],uint16_t Out_Buffer[]);
void SVC_init(void);
#endif /* EQUALIZER_H_ */
