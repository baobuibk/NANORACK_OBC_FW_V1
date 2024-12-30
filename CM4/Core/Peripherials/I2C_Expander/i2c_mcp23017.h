/*
 * i2c_mcp23017.h
 *
 *  Created on: Dec 15, 2024
 *      Author: CAO HIEU
 */

#ifndef PERIPHERIALS_I2C_EXPANDER_I2C_MCP23017_H_
#define PERIPHERIALS_I2C_EXPANDER_I2C_MCP23017_H_

#include "define.h"
#include "../I2C/i2c_it.h"

// Addresses (A0-A2)
#define MCP23017_ADD_20			0x20
#define MCP23017_ADD_21			0x21
#define MCP23017_ADD_22			0x22
#define MCP23017_ADD_23			0x23
#define MCP23017_ADD_24			0x24
#define MCP23017_ADD_25			0x25
#define MCP23017_ADD_26			0x26
#define MCP23017_ADD_27			0x27

/*INPUTS/OUTPUTS */
#define MCP23017_GPA0_Pin		0x00
#define MCP23017_GPA1_Pin		0x01
#define MCP23017_GPA2_Pin		0x02
#define MCP23017_GPA3_Pin		0x03
#define MCP23017_GPA4_Pin		0x04
#define MCP23017_GPA5_Pin		0x05
#define MCP23017_GPA6_Pin		0x06
#define MCP23017_GPA7_Pin		0x07
#define MCP23017_GPB0_Pin		0x08
#define MCP23017_GPB1_Pin		0x09
#define MCP23017_GPB2_Pin		0x0A
#define MCP23017_GPB3_Pin		0x0B
#define MCP23017_GPB4_Pin		0x0C
#define MCP23017_GPB5_Pin		0x0D
#define MCP23017_GPB6_Pin		0x0E
#define MCP23017_GPB7_Pin		0x0F

typedef enum {
	MCP23017Port_A,
	MCP23017Port_B
} MCP23017Port_t;

typedef enum{
	MCP23017_PIN_MODE_INPUT,
	MCP23017_PIN_MODE_INPUT_PULLUP,
	MCP23017_PIN_MODE_OUTPUT
} MCP23017_PinModeIO_t;

typedef enum {
	MCP23017_PIN_POLARITY_NORMAL,
	MCP23017_PIN_POLARITY_INVERTED
} MCP23017_PinPolarity_t;

Peripheral_StatusTypeDef MCP23017_Init(void);
Peripheral_StatusTypeDef MCP23017_PinMode(uint8_t pin, MCP23017_PinModeIO_t mode, MCP23017_PinPolarity_t polarity);
Peripheral_StatusTypeDef MCP23017_PortMode(MCP23017Port_t port, MCP23017_PinModeIO_t pinmode, uint8_t inverted);
Peripheral_StatusTypeDef MCP23017_DigitalRead(uint8_t pin, uint8_t *data);
Peripheral_StatusTypeDef MCP23017_DigitalWrite(uint8_t pin, GPIO_PinState pinState);
Peripheral_StatusTypeDef MCP23017_UpdateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);

#endif /* PERIPHERIALS_I2C_EXPANDER_I2C_MCP23017_H_ */
