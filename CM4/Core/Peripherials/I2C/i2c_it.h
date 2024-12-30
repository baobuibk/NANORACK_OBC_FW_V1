/*
 * i2c.h
 *
 *  Created on: Nov 27, 2024
 *      Author: CAO HIEU
 */

#ifndef PERIPHERIALS_I2C_I2C_IT_H_
#define PERIPHERIALS_I2C_I2C_IT_H_

#include "define.h"
#include "stm32h7xx_ll_i2c.h"


#define I2C_IT_ERRI                     I2C_CR1_ERRIE
#define I2C_IT_TCI                      I2C_CR1_TCIE
#define I2C_IT_STOPI                    I2C_CR1_STOPIE
#define I2C_IT_NACKI                    I2C_CR1_NACKIE
#define I2C_IT_ADDRI                    I2C_CR1_ADDRIE
#define I2C_IT_RXI                      I2C_CR1_RXIE
#define I2C_IT_TXI                      I2C_CR1_TXIE
/**
  * @}
  */

/** @defgroup I2C_Flag_definition I2C Flag definition
  * @{
  */
#define I2C_FLAG_TXE                    I2C_ISR_TXE
#define I2C_FLAG_TXIS                   I2C_ISR_TXIS
#define I2C_FLAG_RXNE                   I2C_ISR_RXNE
#define I2C_FLAG_ADDR                   I2C_ISR_ADDR
#define I2C_FLAG_AF                     I2C_ISR_NACKF
#define I2C_FLAG_STOPF                  I2C_ISR_STOPF
#define I2C_FLAG_TC                     I2C_ISR_TC
#define I2C_FLAG_TCR                    I2C_ISR_TCR
#define I2C_FLAG_BERR                   I2C_ISR_BERR
#define I2C_FLAG_ARLO                   I2C_ISR_ARLO
#define I2C_FLAG_OVR                    I2C_ISR_OVR
#define I2C_FLAG_PECERR                 I2C_ISR_PECERR
#define I2C_FLAG_TIMEOUT                I2C_ISR_TIMEOUT
#define I2C_FLAG_ALERT                  I2C_ISR_ALERT
#define I2C_FLAG_BUSY                   I2C_ISR_BUSY
#define I2C_FLAG_DIR                    I2C_ISR_DIR

typedef enum
{
  I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  I2C_STATE_READY             = 0x01U,   /*!< Peripheral Initialized and ready for use  */
  I2C_STATE_BUSY              = 0x02U,   /*!< An internal process is ongoing            */
  I2C_STATE_BUSY_TX           = 0x03U,   /*!< Data Transmission process is ongoing      */
  I2C_STATE_BUSY_RX           = 0x04U,   /*!< Data Reception process is ongoing         */
  I2C_STATE_LISTEN            = 0x05U,   /*!< Address Listen Mode is ongoing            */
  I2C_STATE_BUSY_TX_LISTEN    = 0x06U,   /*!< Address Listen Mode and Data Transmission
                                                 process is ongoing                         */
  I2C_STATE_BUSY_RX_LISTEN    = 0x07U,   /*!< Address Listen Mode and Data Reception
                                                 process is ongoing                         */
  I2C_STATE_ABORT             = 0x08U,   /*!< Abort user request ongoing                */

} LL_I2C_StateTypeDef;

typedef enum
{
  I2C_DONE             		  = 0x10U,
  I2C_START_SB                = 0x11U,
  I2C_ADDRSENT_ADDR           = 0x12U,
  I2C_REGSENT_TXIS            = 0x13U,
  I2C_TRANSMDATA_TXIS         = 0x14U,
  I2C_REREADDATA_RERX         = 0x15U,
  I2C_READWAITING    		  = 0x16U,
  I2C_READPROCESSING          = 0x17U,
  I2C_TRANSFIN_TCR            = 0x18U,
  I2C_STOP_SF				  = 0x19U,
} I2C_Process_StateTypeDef;

typedef struct LL_I2C_HandleTypeDef
{
  I2C_TypeDef               			 	*Instance;      		/*!< I2C registers base address                */

  uint8_t                    				*pBuffer;      			/*!< Pointer to I2C transfer buffer            */

  volatile uint8_t              			vSize;     				/*!< I2C transfer size counter                 */

  volatile LL_I2C_StateTypeDef  			PreviousState;  		/*!< I2C communication Previous state          */

  volatile LL_I2C_StateTypeDef  			State;          		/*!< I2C communication state                   */

  volatile I2C_Process_StateTypeDef         Process; 				/*!< I2C Processing Event counter              */

  volatile uint8_t              			SlaveAddress;     		/*!< I2C Target device address                 */

  volatile uint8_t              			RegAddress;     		/*!< I2C Target register address               */

  Peripheral_StatusTypeDef					ErrorCode;				/*!< I2C Error code when running               */

} LL_I2C_HandleTypeDef;

Peripheral_StatusTypeDef I2C_Mem_Write_IT(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
																uint8_t *pData, uint8_t Size);
Peripheral_StatusTypeDef I2C_Mem_Read_IT(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
                                       	   	   	   	   	   	    uint8_t *pData, uint8_t Size);
void I2C_EV_IRQHandler(LL_I2C_HandleTypeDef *i2c);

Peripheral_StatusTypeDef I2C_Write(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Length);
Peripheral_StatusTypeDef I2C_Read(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Length);


Peripheral_StatusTypeDef I2C_Read_NOSTOP(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Length);

#endif /* PERIPHERIALS_I2C_I2C_IT_H_ */
