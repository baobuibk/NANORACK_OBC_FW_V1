/*
 * i2c_mcp23017.c
 *
 *  Created on: Dec 15, 2024
 *      Author: CAO HIEU
 */

#include "i2c_mcp23017.h"

#define MCP23017_ADDRESS      0x23

extern LL_I2C_HandleTypeDef ll_i2c4;

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define MCP23017_IODIRA 		0x00	///< Controls the direction of the data I/O for port A.
#define MCP23017_IPOLA 			0x02	///< Configures the polarity on the corresponding GPIO_ port bits for port A.
#define MCP23017_GPINTENA 		0x04	///< Controls the interrupt-on-change for each pin of port A.
#define MCP23017_DEFVALA 		0x06	///< Controls the default comparaison value for interrupt-on-change for port A.
#define MCP23017_INTCONA 		0x08	///< Controls how the associated pin value is compared for the interrupt-on-change for port A.
#define MCP23017_IOCONA 		0x0A	///< Controls the device.
#define MCP23017_GPPUA 			0x0C	///< Controls the pull-up resistors for the port A pins.
#define MCP23017_INTFA 			0x0E	///< Reflects the interrupt condition on the port A pins.
#define MCP23017_INTCAPA 		0x10	///< Captures the port A value at the time the interrupt occured.
#define MCP23017_GPIOA 			0x12	///< Reflects the value on the port A.
#define MCP23017_OLATA 			0x14	///< Provides access to the port A output latches.

#define MCP23017_IODIRB 		0x01	///< Controls the direction of the data I/O for port B.
#define MCP23017_IPOLB 			0x03	///< Configures the polarity on the corresponding GPIO_ port bits for port B.
#define MCP23017_GPINTENB 		0x05	///< Controls the interrupt-on-change for each pin of port B.
#define MCP23017_DEFVALB 		0x07	///< Controls the default comparaison value for interrupt-on-change for port B.
#define MCP23017_INTCONB 		0x09	///< Controls how the associated pin value is compared for the interrupt-on-change for port B.
#define MCP23017_IOCONB 		0x0B	///< Controls the device.
#define MCP23017_GPPUB 			0x0D	///< Controls the pull-up resistors for the port B pins.
#define MCP23017_INTFB 			0x0F	///< Reflects the interrupt condition on the port B pins.
#define MCP23017_INTCAPB 		0x11	///< Captures the port B value at the time the interrupt occured.
#define MCP23017_GPIOB 			0x13	///< Reflects the value on the port B.
#define MCP23017_OLATB 			0x15	///< Provides access to the port B output latches.

#define MCP23017_INT_ERR		0xFF

static uint8_t MCP23017_RegForPin (uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
	return(pin<8) ? portAaddr : portBaddr;
}

static uint8_t MCP23017_BitForPin(uint8_t pin){
	return pin % 8;
}

static Peripheral_StatusTypeDef MCP23017_ReadRegister(uint8_t addr, uint8_t *data);
static Peripheral_StatusTypeDef MCP23017_WriteRegister(uint8_t regAddr, uint8_t regValue);


Peripheral_StatusTypeDef MCP23017_UpdateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
	uint8_t regValue;
	uint8_t regAddr = MCP23017_RegForPin(pin,portAaddr,portBaddr);
	uint8_t bit = MCP23017_BitForPin(pin);
	MCP23017_ReadRegister(regAddr, &regValue);

	// set the value for the particular bit
	bitWrite(regValue,bit,pValue);

	return (MCP23017_WriteRegister(regAddr,regValue));
}

Peripheral_StatusTypeDef MCP23017_ReadRegister(uint8_t addr, uint8_t *data) {
	Peripheral_StatusTypeDef ret;
	// read the current GPINTEN
	ret = I2C_Read(&ll_i2c4, MCP23017_ADDRESS, addr, data, 1);
	return ret;
}

static Peripheral_StatusTypeDef MCP23017_WriteRegister(uint8_t regAddr, uint8_t regValue)
{
	Peripheral_StatusTypeDef ret;
	ret = I2C_Write(&ll_i2c4, MCP23017_ADDRESS, regAddr, &regValue, 1);
	return ret;
}

Peripheral_StatusTypeDef MCP23017_Init(void)
{
	Peripheral_StatusTypeDef ret = Status_ERROR;

	//BANK = 	0 : sequential register addresses
	//MIRROR = 	0 : use configureInterrupt
	//SEQOP = 	1 : sequential operation disabled, address pointer does not increment
	//DISSLW = 	0 : slew rate enabled
	//HAEN = 	0 : hardware address pin is always enabled on 23017
	//ODR = 	0 : open drain output
	//INTPOL = 	0 : interrupt active low
	// bit0 - unplemented read as 0
//	uint8_t byte = 0 | 1 << 5 | 1 << 6;
//
//	for (uint8_t i = 0; i < 2; i++) {
//		ret = MCP23017_WriteRegister(hdev, MCP23017_IOCONA+i, byte);
//		if (ret != Status_OK) return ret;
//	}
	ret = MCP23017_PortMode(MCP23017Port_A, MCP23017_PIN_MODE_OUTPUT, MCP23017_PIN_POLARITY_NORMAL);
	if (ret != Status_OK) return ret;
	ret = MCP23017_PortMode(MCP23017Port_B, MCP23017_PIN_MODE_OUTPUT, MCP23017_PIN_POLARITY_NORMAL);
	if (ret != Status_OK) return ret;
	HAL_Delay(50);
	ret = MCP23017_PinMode(MCP23017_GPB1_Pin, MCP23017_PIN_MODE_INPUT, MCP23017_PIN_POLARITY_NORMAL);
	if (ret != Status_OK) return ret;
//	ret = MCP23017_PinMode(MCP23017_GPB2_Pin, MCP23017_PIN_MODE_OUTPUT, MCP23017_PIN_POLARITY_NORMAL);
//	if (ret != Status_OK) return ret;

	return ret;
}

Peripheral_StatusTypeDef MCP23017_PortMode(MCP23017Port_t port, MCP23017_PinModeIO_t pinmode, MCP23017_PinPolarity_t pinpolarity)
{
	Peripheral_StatusTypeDef ret;

	ret = MCP23017_WriteRegister(MCP23017_IODIRA + port,
			(pinmode == MCP23017_PIN_MODE_INPUT || pinmode == MCP23017_PIN_MODE_INPUT_PULLUP) ? 0xFF : 0x00);
	if (ret != Status_OK) return ret;

	ret = MCP23017_WriteRegister(MCP23017_GPPUA + port, (pinmode == MCP23017_PIN_MODE_INPUT_PULLUP ? 0xff : 0x00));
	if (ret != Status_OK) return ret;

	ret = MCP23017_WriteRegister(MCP23017_IPOLA + port, pinpolarity ? 0xff : 0x00);
	if (ret != Status_OK) return ret;

	return Status_OK;
}

Peripheral_StatusTypeDef MCP23017_PinMode(uint8_t pin, MCP23017_PinModeIO_t mode, MCP23017_PinPolarity_t polarity)
{
	Peripheral_StatusTypeDef ret;

	/* if input we set IODIRA or IODIRB (depending on pin number) to 1 */
	ret = MCP23017_UpdateRegisterBit(pin, (mode == MCP23017_PIN_MODE_INPUT || mode == MCP23017_PIN_MODE_INPUT_PULLUP),
			MCP23017_IODIRA, MCP23017_IODIRB);
	if (ret != Status_OK) return ret;

	/* we also need to control the polarity, will be ignored anyway if output, 1 if pol inverted */
	ret = MCP23017_UpdateRegisterBit(pin, polarity == MCP23017_PIN_POLARITY_INVERTED, MCP23017_IPOLA, MCP23017_IPOLB);
	if (ret != Status_OK) return ret;

	/* then we need to setup the pull up for that pin if MCP23017_PIN_MODE_INPUT_PULLUP was selected */
	ret = MCP23017_UpdateRegisterBit(pin, mode == MCP23017_PIN_MODE_INPUT_PULLUP, MCP23017_GPPUA, MCP23017_GPPUB);
	if (ret != Status_OK) return ret;

	return Status_OK;
}

Peripheral_StatusTypeDef MCP23017_DigitalRead(uint8_t pin, uint8_t *data) {
	uint8_t bit = MCP23017_BitForPin(pin);
	uint8_t regAddr = MCP23017_RegForPin(pin, MCP23017_GPIOA, MCP23017_GPIOB);
	Peripheral_StatusTypeDef ret = MCP23017_ReadRegister(regAddr, data);
	*data = (*data >> bit) & 0x1;
	return ret;
}

Peripheral_StatusTypeDef MCP23017_DigitalWrite(uint8_t pin, GPIO_PinState pinState)
{
	uint8_t data;
	Peripheral_StatusTypeDef ret;
	uint8_t bit = MCP23017_BitForPin(pin);

	// read the current GPIO output latches
	uint8_t regAddr = MCP23017_RegForPin(pin, MCP23017_OLATA, MCP23017_OLATB);
	ret = MCP23017_ReadRegister(regAddr, &data);
	if (ret != Status_OK) return ret;

	// set the pin and direction
	bitWrite(data, bit, pinState);

	// write the new GPIO
	regAddr = MCP23017_RegForPin(pin, MCP23017_GPIOA, MCP23017_GPIOB);
	return (MCP23017_WriteRegister(regAddr, data));
}
