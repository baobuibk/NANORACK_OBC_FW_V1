/*
 * spi_ads8327.c
 *
 *  Created on: Dec 14, 2024
 *      Author: CAO HIEU
 */

#include "spi.h"
#include "spi_ads8327.h"
#include <stdio.h>
#include "../I2C_Expander/i2c_mcp23017.h"
#include "stm32h7xx_ll_spi.h"

#define ADC_SPI	SPI6
#define ADS8327_CONVST_PIN  MCP23017_GPB2_Pin

/*	[ADS8327]
 *  CFR: Configuration Register
 *  https://ssihla.wordpress.com/wp-content/uploads/2013/08/cfr.png
 *
 *	D[11:0]
 *	D11: Channel Select Mode: 				0-Manual 		|		 1-Auto				| Default: 1
 *  D10: Conversion clock source: 			0-SCLK/2		|		 1-Internal OSC		| Default: 1
 *  D9 : Trigger							0-Auto			|		 1-Manual & CONVST  | Default: 1
 *  D8 : Reversed																		| Default: 0
 *
 *  																					Hex---> E
 *
 *  D7 : EOC/INT							0-ActiveHigh    |		 1-ActiveLow		| Default: 1
 *  D6 : EOC/INT Function Pin				0-PinAsINT		|		 1-PinAsEOC			| Default: 1
 *  D5 : Chain-mode							0-Enable		|        1-Disable			| Default: 1
 *  D4 : Auto-nap Power-down				0-Enable		|		 1-Disable			| Default: 1
 *
 *  																					Hex---> F
 *
 *  D3 : Nap Power-down mode				0-Enable		|		 1-ResumeThisMode	| Default: 1
 *  D2 : Deep-power-down					0-Enable		|		 1-Disable			| Default: 1
 *  D1 : TAG Bit																ADS8327   Default: 0
 *  D0 : Reset								0-SystemReset   |		 1-Normal Operation | Default: 1
 *
 *  																					Hex---> D
 * */

// NOTE: pinSet(ADS8327_CONVST_PORT, ADS8327_CONVST_PIN); Must be high
// NOTE: pinSet(ADS8327_CONVST_PORT, ADS8327_CONVST_PIN); Must be high
static inline void ADS8327_SPI_Select(void){
	LL_GPIO_ResetOutputPin(SPI6_ISO_CS_GPIO_Port, SPI6_ISO_CS_Pin);
}

static inline void ADS8327_SPI_Deselect(void){
	LL_GPIO_SetOutputPin(SPI6_ISO_CS_GPIO_Port, SPI6_ISO_CS_Pin);
}

void ADS8327_Init(void)
{
    uint16_t config = ADS8327_ConfigRegRead();
    printf("Config: 0b");
    for (int i = 15; i >= 0; i--) {
        printf("%d", (config >> i) & 1);
    }
    printf(" 0x%04X\r\n", config);
//    ADS8327_RegWrite16(WRITE_CFR, config);

    // channel 0 active by default
}

uint16_t ADS8327_ConfigRegRead(void)
{
    uint16_t result;

    // Send first byte, ignore 4 msBits, get  12 bits after

    uint8_t array[2];
    array[0] = READ_CFR << 4;
    array[1] = 0xFF;

    uint8_t data[2];
    data[0] = 0;
    data[1] = 0;

    ADS8327_SPI_Select();
    for(int i = 0; i < sizeof(array); i++){
    	data[i] = SPI_LL_Transmit(ADC_SPI, array[i]);
    }
    ADS8327_SPI_Deselect();


    result = data[0];
    result <<= 8;
    result |= data[1];



    // use 12 least-significant bits
    result &= 0x0fff;
    return result;
}

void ADS8327_RegWrite16(uint8_t address, uint16_t data)
{
    uint8_t array[2];
    array[0] = (address << 4) | ((data >> 8) & 0xf);
    array[1] = data & 0xff;
    ADS8327_SPI_Select();
    for(int i = 0; i < sizeof(array); i++){
    	SPI_LL_Write(ADC_SPI, array[i]);
    }
    ADS8327_SPI_Deselect();
}

void ADS8327_SelectChannel(uint8_t channel)
{
    // select channel
	ADS8327_RegWrite4(channel == REG_CHANNEL0 ?
    							REG_CHANNEL0 :
								REG_CHANNEL1);
}

void ADS8327_RegWrite4(uint8_t address)
{
    uint8_t array[2];
    array[0] = address << 4;
    array[1] = 0;
    ADS8327_SPI_Select();
    for(int i = 0; i < sizeof(array); i++){
    	SPI_LL_Write(ADC_SPI, array[i]);
    }
    ADS8327_SPI_Deselect();
}

/*NOP CALL*/

int8_t ADS8327_Read(uint16_t *value)
{
    // wait for previous conversion to end
    //while (!pinRead(ADS8327_EOC_PORT, ADS8327_EOC_PIN));
    uint8_t dummy[1];
    dummy[0] = READ_DATA << 4;

    ADS8327_SPI_Select();
    for(int i = 0; i < sizeof(dummy); i++){
    	SPI_LL_Write(ADC_SPI, dummy[i]);
    }
    ADS8327_SPI_Deselect();


    // start the conversion
	MCP23017_DigitalWrite(ADS8327_CONVST_PIN, GPIO_PIN_RESET);
//    pinClear(ADS8327_CONVST_PORT, ADS8327_CONVST_PIN);
//
	uint8_t abc;
    (void)MCP23017_DigitalRead(MCP23017_GPB1_Pin, &abc);
    printf("DataPin 1st: %d\r\n", abc);
    __asm__("NOP");
    __asm__("NOP");
    __asm__("NOP");

//
	MCP23017_DigitalWrite(ADS8327_CONVST_PIN, GPIO_PIN_SET);
//    pinSet(ADS8327_CONVST_PORT, ADS8327_CONVST_PIN);
//
//    // wait for end of conversion
//    while (!pinRead(ADS8328_EOC_PORT, ADS8328_EOC_PIN));
//

    (void)MCP23017_DigitalRead(MCP23017_GPB1_Pin, &abc);
    printf("DataPin2nd: %d\r\n", abc);

    __asm__("NOP");
    __asm__("NOP");
    __asm__("NOP");
    uint8_t array[2];
    array[0]=0;
	array[1]=0;
    ADS8327_SPI_Select();

    for(int i = 0; i < sizeof(array); i++){
    	array[i] = SPI_LL_Transmit(ADC_SPI, array[i]);
    }
    ADS8327_SPI_Deselect();


    *value = array[0];
    *value <<= 8;
    *value |= array[1];

    return 0;
}
