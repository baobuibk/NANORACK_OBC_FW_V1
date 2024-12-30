/*
 * spi_ads8327.h
 *
 *  Created on: Dec 14, 2024
 *      Author: CAO HIEU
 */
#include "main.h"

#ifndef PERIPHERIALS_SPI_ADC_SPI_ADS8327_H_
#define PERIPHERIALS_SPI_ADC_SPI_ADS8327_H_

#define CHANNEL0 0x0
#define CHANNEL1 0x1
#define WAKE_UP 0xB
#define READ_CFR 0xC
#define READ_DATA 0xD
#define WRITE_CFR 0xE
#define DEFAULT_MODE 0xF

#define REG_CHANNEL0       0x00
#define REG_CHANNEL1       0x01

#define CFG_REG_MASK 0x0FFF

#define COMMAND_OFFSET 12

#define RESET_OFFSET 0
#define TAG_BIT_OFFSET 1
#define DEEP_PWR_DOWN_OFFSET 2
#define NAP_PWR_DOWN_OFFSET 3
#define AUTO_NAP_PWR_DWN_OFFSET 4
#define CHAIN_MODE_OFFSET 5
#define EOC_OFFSET 6
#define EOC_POLARITY_OFFSET 7
#define DONT_CARE_OFFSET 8
#define TRIGGER_SOURCE_OFFSET 9
#define CLOCK_SOURCE_OFFSET 10
#define CHANNEL_SELECT_MODE_OFFSET 11

uint16_t ADS8327_ConfigRegRead(void);
void ADS8327_RegWrite16(uint8_t address, uint16_t data);
void ADS8327_SelectChannel(uint8_t channel);
void ADS8327_RegWrite4(uint8_t address);
int8_t ADS8327_Read(uint16_t *value);
void ADS8327_Init(void);

#endif /* PERIPHERIALS_SPI_ADC_SPI_ADS8327_H_ */
