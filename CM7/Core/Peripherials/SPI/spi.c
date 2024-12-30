/*
 * spi.c
 *
 *  Created on: Nov 27, 2024
 *      Author: CAO HIEU
 */

#include "spi.h"
#include "stdio.h"
#include "usbd_cdc_if.h"

#define SPI_TIMEOUT 500000

uint8_t SPI_LL_Transmit(SPI_TypeDef *SPIx, uint8_t data){
	uint32_t timeout = SPI_TIMEOUT;
	LL_SPI_TransmitData8(SPIx, data);

	while (!LL_SPI_IsActiveFlag_TXC(SPIx)) {
	    if (timeout == 0) {
	        CDC_SendString("SPI Timeout during TX getting data\r\n");
	        return ERROR_SPI_TRANS;
	    }
	    timeout--;
	}
	timeout = SPI_TIMEOUT;
	while (!LL_SPI_IsActiveFlag_RXWNE(SPIx)) {
	    if (timeout == 0) {
	        CDC_SendString("SPI Timeout during RX getting data\r\n");
	        return ERROR_SPI_RECV;
	    }
	    timeout--;
	}

	HAL_Delay(1);
//    	  CDC_SendString("Timeout Cost: %lu", (SPI_TIMEOUT - timeout));

	return LL_SPI_ReceiveData8(SPIx);
}

uint16_t SPI_LL_Transmit16(SPI_TypeDef *SPIx, uint16_t data){
	uint32_t timeout = SPI_TIMEOUT;
	LL_SPI_TransmitData16(SPIx, data);

	while(LL_SPI_IsActiveFlag_TXC(SPIx) && timeout--);
    if (timeout == 0) {
        CDC_SendString("SPI Timeout during TX getting data\r\n");
        return ERROR_SPI_TRANS;
    }

    while(LL_SPI_IsActiveFlag_RXWNE(SPIx) && timeout--);
    if (timeout == 0) {
        CDC_SendString("SPI Timeout during RX getting data\r\n");
        return ERROR_SPI_RECV;
    }
//    	  CDC_SendString("Timeout Cost: %lu", (SPI_TIMEOUT - timeout));
//    CDC_SendString("Hello!");
	return LL_SPI_ReceiveData16(SPIx);
}

int8_t SPI_LL_Write(SPI_TypeDef *SPIx, uint8_t data){
	uint32_t timeout = SPI_TIMEOUT;
	LL_SPI_TransmitData8(SPIx, data);

	while (LL_SPI_IsActiveFlag_TXC(SPIx)) {
	    if (timeout == 0) {
	        CDC_SendString("SPI Timeout during getting data\r\n");
	        return -1;
	    }
	    timeout--;
	}

    LL_SPI_ReceiveData8(SPIx);
    return 0;
}

void SPI_LL_TransmitArray(SPI_TypeDef *SPIx, uint8_t *data, uint16_t size) {
    for (uint8_t i = 0; i < size; i++) {
        SPI_LL_Write(SPIx, data[i]);
    }
}

