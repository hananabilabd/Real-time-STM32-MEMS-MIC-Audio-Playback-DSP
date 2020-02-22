/*
 * codec_WM8731.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Hanna Nabil
 */


/*
 * codec.c - Discovery board codec interface routines
 *
 * Cut from stm32f4_discovery_audio_codec.c
 *
 */

/*========================

                CS43L22 Audio Codec Control Functions
                                                ==============================*/
/**
  * @brief  Initializes the audio codec and all related interfaces (control
  *         interface: I2C and audio interface: I2S)
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */

#include "codec_WM8731.h"

extern I2C_HandleTypeDef hi2c1;
static uint8_t iData[2];

/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400

/* Codec audio Standards */
#ifdef I2S_STANDARD_PHILLIPS
 #define  CODEC_STANDARD                0x04
 #define I2S_STANDARD                   I2S_Standard_Phillips
#elif defined(I2S_STANDARD_MSB)
 #define  CODEC_STANDARD                0x00
 #define I2S_STANDARD                   I2S_Standard_MSB
#elif defined(I2S_STANDARD_LSB)
 #define  CODEC_STANDARD                0x08
 #define I2S_STANDARD                   I2S_Standard_LSB
#else
 #error "Error: No audio communication standard selected !"
#endif /* I2S_STANDARD */

#define W8731_ADDR_0 0x1A // this is the address of the CODEC when CSB is low
#define W8731_ADDR_1 0x1B // this is the address of the CODEC when CSB is high
#define W8731_NUM_REGS 10
/* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS           (W8731_ADDR_0<<1)

const uint16_t w8731_init_data[] =
{
	0x097,	// Reg 00: Left Line In (0dB, mute off)
	0x097,	// Reg 01: Right Line In (0dB, mute off)
	0x07F,	// Reg 02: Left Headphone out (0dB)
	0x07F,	// Reg 03: Right Headphone out (0dB)
	0x012,	// Reg 04: Analog Audio Path Control (DAC sel, Mute Mic) //0x012 in old version
	0x000,	// Reg 05: Digital Audio Path Control
	0x067,	// Reg 06: Power Down Control (Clkout, Osc, Mic Off) //0x062 in old version
//	0x00E,	// Reg 07: Digital Audio Interface Format (i2s, 32-bit, slave)
	0x002,	// Reg 07: Digital Audio Interface Format (i2s, 16-bit, slave)
	0x000,	// Reg 08: Sampling Control (Normal, 256x, 48k ADC/DAC)
	0x001	// Reg 09: Active Control
};


uint32_t Codec_Init(uint32_t AudioFreq)
{
	uint32_t counter = 0;

	/* Configure the Codec related IOs */
	//Codec_GPIO_Init();

	/* Initialize the Control interface of the Audio Codec */
	//Codec_CtrlInterface_Init();

	/* Configure the I2S peripheral */
	//Codec_AudioInterface_Init(AudioFreq);

	/* Reset the Codec Registers */
	Codec_Reset();

	/* Return communication control value */
	return counter;
}

/**
  * @brief  Resets the audio codec. It restores the default configuration of the
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
void Codec_Reset(void)
{
	uint8_t i;

	Codec_WriteRegister(0x0F, 0x00);

	/* Load default values */
	for(i=0;i<W8731_NUM_REGS;i++)
	{
		Codec_WriteRegister(i, w8731_init_data[i]);
	}
}

/**
  * @brief  Writes a Byte to a given register into the audio codec through the
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue)
{
	uint32_t result = 0;
	// RegisterValue is 9-bit register
	/* Assemble 2-byte data in WM8731 format */
	uint8_t Byte1 = ((RegisterAddr<<1)&0xFE) | ((RegisterValue>>8)&0x1);
	uint8_t Byte2 = RegisterValue&0xFF;
	iData[0] = Byte1;
	iData[1] = Byte2;
	/* Transmit the slave address and enable writing operation */
	// take the most significant 7 bit address in the address byte
	if (HAL_I2C_Master_Transmit(&hi2c1, CODEC_ADDRESS, iData, 2, 400) != HAL_OK)
		{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
		}
	else {
		result =1;
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	}
	//I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

	/* Return the verifying value: 0 (Passed) or 1 (Failed) */
	return result;
}

