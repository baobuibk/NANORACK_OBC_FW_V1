/*
 * spi.h
 *
 *  Created on: Nov 27, 2024
 *      Author: CAO HIEU
 */

#ifndef PERIPHERIALS_SPI_SPI_H_
#define PERIPHERIALS_SPI_SPI_H_

#include "stm32h7xx_ll_spi.h"

#define ERROR_SPI_TRANS					(0xFE)
#define ERROR_SPI_RECV					(0xFF)

uint8_t SPI_LL_Transmit(SPI_TypeDef *SPIx, uint8_t data);
void SPI_LL_TransmitArray(SPI_TypeDef *SPIx, uint8_t *data, uint16_t size);
int8_t SPI_LL_Write(SPI_TypeDef *SPIx, uint8_t data);
uint16_t SPI_LL_Transmit16(SPI_TypeDef *SPIx, uint16_t data);

#endif /* PERIPHERIALS_SPI_SPI_H_ */
