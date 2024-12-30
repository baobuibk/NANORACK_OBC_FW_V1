/*
 * i2c.c
 *
 *  Created on: Nov 27, 2024
 *      Author: CAO HIEU
 */

#include "i2c_it.h"
#include "stdio.h"
#include "stm32h7xx_ll_i2c.h"
#include "main.h"

#define I2C_TIMEOUT		20		/* 10ms */

static inline uint32_t LL_I2C_IsBusyIT(const I2C_TypeDef *I2Cx)
{
  return ((READ_BIT(I2Cx->ISR, LL_I2C_ISR_BUSY) == LL_I2C_ISR_BUSY) ? 1UL : 0UL);
}

Peripheral_StatusTypeDef I2C_Write(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Length)
{
	uint32_t tickstart;
    tickstart = HAL_GetTick();
    while(LL_I2C_IsActiveFlag_BUSY(i2c->Instance))
    {
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
		      return Status_BUSY;
		}
    }
    LL_I2C_HandleTransfer((i2c->Instance), (Slave_address << 1), LL_I2C_ADDRSLAVE_7BIT, Length+1 ,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE );

    tickstart = HAL_GetTick();

	while(!LL_I2C_IsActiveFlag_TXIS(i2c->Instance)){
		/* check if slave acknowledged */
		if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
			return Status_ERROR;
		}
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
			return Status_TIMEOUT;
		}
	}

	LL_I2C_TransmitData8((i2c->Instance), Reg_address);

	tickstart = HAL_GetTick();

	while(!LL_I2C_IsActiveFlag_TXIS(i2c->Instance)){
		/* check if slave acknowledged */
		if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
			return Status_ERROR;
		}
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
			return Status_TIMEOUT;
		}
	}

    for(uint8_t i = 0; i < Length; i++)
    {
        // check if the TXIS flag is set
    	while(!LL_I2C_IsActiveFlag_TXIS(i2c->Instance)){
    		/* check if slave acknowledged */
    		if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
    			return Status_ERROR;
    		}
    		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
    			I2C4_ReInit();
    			return Status_TIMEOUT;
    		}
    	}
        // Write in the DR register the data to be sent
        LL_I2C_TransmitData8((i2c->Instance), pData[i]);

    }

	while(!LL_I2C_IsActiveFlag_TXE(i2c->Instance)){
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
			return Status_TIMEOUT;
		}
	}

	return Status_OK;
}

Peripheral_StatusTypeDef I2C_Read_NOSTOP(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
                                           uint8_t *pData, uint8_t Length)
{
    uint32_t tickstart;

    // 1. Chờ đến khi I2C không bận
    tickstart = HAL_GetTick();
    while (LL_I2C_IsActiveFlag_BUSY(i2c->Instance))
    {
        if (HAL_GetTick() - tickstart > I2C_TIMEOUT)
        {
            I2C4_ReInit();
            return Status_BUSY;
        }
    }

    // 2. Gửi START condition + Địa chỉ slave (Write mode)
    LL_I2C_HandleTransfer(i2c->Instance, Slave_address << 1, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    tickstart = HAL_GetTick();
    while (!LL_I2C_IsActiveFlag_TXIS(i2c->Instance))
    {
        if (LL_I2C_IsActiveFlag_NACK(i2c->Instance))
            return Status_ERROR;
        if (HAL_GetTick() - tickstart > I2C_TIMEOUT)
        {
            I2C4_ReInit();
            return Status_TIMEOUT;
        }
    }

    // 3. Gửi địa chỉ thanh ghi
    LL_I2C_TransmitData8(i2c->Instance, Reg_address);

    tickstart = HAL_GetTick();
    while (!LL_I2C_IsActiveFlag_TC(i2c->Instance)) // Chờ hoàn thành gửi địa chỉ thanh ghi
    {
        if (HAL_GetTick() - tickstart > I2C_TIMEOUT)
        {
            I2C4_ReInit();
            return Status_TIMEOUT;
        }
    }

    // 4. Gửi START condition + Địa chỉ slave (Read mode)
    LL_I2C_HandleTransfer(i2c->Instance, Slave_address << 1, LL_I2C_ADDRSLAVE_7BIT, Length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    // 5. Đọc dữ liệu từ RXDR
    for (int i = 0; i < Length; i++)
    {
        tickstart = HAL_GetTick();
        while (!LL_I2C_IsActiveFlag_RXNE(i2c->Instance))
        {
            if (LL_I2C_IsActiveFlag_NACK(i2c->Instance))
                return Status_ERROR;
            if (HAL_GetTick() - tickstart > I2C_TIMEOUT)
            {
                I2C4_ReInit();
                return Status_TIMEOUT;
            }
        }
        pData[i] = LL_I2C_ReceiveData8(i2c->Instance);
    }

    // 6. Chờ STOP condition
    tickstart = HAL_GetTick();
    while (!LL_I2C_IsActiveFlag_STOP(i2c->Instance))
    {
        if (HAL_GetTick() - tickstart > I2C_TIMEOUT)
        {
            I2C4_ReInit();
            return Status_TIMEOUT;
        }
    }
    LL_I2C_ClearFlag_STOP(i2c->Instance);

    return Status_OK;
}



Peripheral_StatusTypeDef I2C_Read(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Length)
{
	uint32_t tickstart;
    tickstart = HAL_GetTick();
    while(LL_I2C_IsActiveFlag_BUSY(i2c->Instance))
    {
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
		      return Status_BUSY;
		}
    }

    LL_I2C_HandleTransfer((i2c->Instance), (Slave_address << 1), LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);

    tickstart = HAL_GetTick();

	while(!LL_I2C_IsActiveFlag_TXIS(i2c->Instance)){
		/* check if slave acknowledged */
		if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
			return Status_ERROR;
		}
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();

			return Status_TIMEOUT;
		}
	}

	LL_I2C_TransmitData8((i2c->Instance), Reg_address);

    tickstart = HAL_GetTick();

	while(!LL_I2C_IsActiveFlag_TXE(i2c->Instance)){
		/* check if slave acknowledged */
		if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
			return Status_ERROR;
		}
		if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
			I2C4_ReInit();
			return Status_TIMEOUT;
		}
	}
    LL_I2C_AcknowledgeNextData((i2c->Instance), LL_I2C_ACK);

    LL_I2C_HandleTransfer((i2c->Instance), (Slave_address << 1), ((Slave_address << 1) | 1), Length, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

    LL_I2C_AcknowledgeNextData((i2c->Instance), LL_I2C_ACK);

    tickstart = HAL_GetTick();

    for(int i = 0; i < Length; i++){
		while(!LL_I2C_IsActiveFlag_RXNE(i2c->Instance)){
			/* check if slave acknowledged */
			if(LL_I2C_IsActiveFlag_NACK(i2c->Instance)){
				return Status_ERROR;
			}
			if(HAL_GetTick() - tickstart > I2C_TIMEOUT){
				I2C4_ReInit();
				return Status_TIMEOUT;
			}
		}
		 pData[i]=LL_I2C_ReceiveData8(i2c->Instance);
    }
    return Status_OK;
}

Peripheral_StatusTypeDef I2C_Mem_Write_IT(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
															uint8_t *pData, uint8_t Size)
{

  /* Check the state */
  if (i2c->State == I2C_STATE_READY)
  {

    if (LL_I2C_IsActiveFlag_BUSY(i2c->Instance))
    {
      return Status_BUSY;
    }
    if (i2c->Process != I2C_DONE)
    {
      return Status_BUSY;
    }
    /* Process Locked */
    i2c->State      = I2C_STATE_BUSY_TX;
    i2c->Process    = I2C_START_SB;
    /* Prepare transfer parameters */
    i2c->RegAddress = Reg_address;
    i2c->pBuffer    = pData;
    i2c->vSize      = Size;
    i2c->SlaveAddress  = Slave_address;
    /* Generate start condition */

    LL_I2C_GenerateStartCondition(I2C1);
    i2c->Process    = I2C_ADDRSENT_ADDR;
    LL_I2C_TransmitData8(I2C1, 0x12);

    /* Enable transfer event */
 //   LL_I2C_HandleTransfer(I2C1, 0, Slave_address, 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
    LL_I2C_EnableIT_TX(i2c->Instance);
    LL_I2C_EnableIT_NACK(i2c->Instance);
    LL_I2C_EnableIT_STOP(i2c->Instance);
    LL_I2C_EnableIT_TC(i2c->Instance);
    LL_I2C_EnableIT_ERR(i2c->Instance);

    printf("ENBALE IRQ!!!\r\n");
    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}

Peripheral_StatusTypeDef I2C_Mem_Read_IT(LL_I2C_HandleTypeDef *i2c, uint8_t Slave_address, uint8_t Reg_address,
                                       uint8_t *pData, uint8_t Size)
{
  /* Check the state */
  if (i2c->State == I2C_STATE_READY)
  {

    if (LL_I2C_IsBusyIT(i2c->Instance))
    {
      return Status_BUSY;
    }
    if (i2c->Process != I2C_DONE)
    {
      return Status_BUSY;
    }
    /* Process Locked */
    i2c->State      = I2C_STATE_BUSY_RX;
    i2c->Process    = I2C_START_SB;
    /* Prepare transfer parameters */
    i2c->pBuffer    = pData;
    i2c->vSize      = Size;
    i2c->SlaveAddress  = Slave_address;
    /* Generate start condition */
    LL_I2C_GenerateStartCondition(i2c->Instance);
    i2c->Process    = I2C_ADDRSENT_ADDR;
    /* Enable transfer event */
    LL_I2C_EnableIT_RX(i2c->Instance);
    return Status_OK;
  }
  else
  {
    return Status_BUSY;
  }
}
uint8_t count = 0;
void I2C_EV_IRQHandler(LL_I2C_HandleTypeDef *i2c)
{
	count++;
	printf("IRQ! [%d]!!!\r\n", count);
    switch (i2c->Process)
    {

        case I2C_ADDRSENT_ADDR:
//            if (!LL_I2C_IsActiveFlag_SB(i2c->Instance)) return;
            LL_I2C_TransmitData8(i2c->Instance, i2c->SlaveAddress);
            i2c->Process = I2C_REGSENT_TXIS;
            break;
        case I2C_REGSENT_TXIS:
            if (!LL_I2C_IsActiveFlag_ADDR(i2c->Instance)) return;
            LL_I2C_ClearFlag_ADDR(i2c->Instance);
            LL_I2C_TransmitData8(i2c->Instance, i2c->RegAddress);
            if(i2c->State == I2C_STATE_BUSY_TX){
                i2c->Process = I2C_TRANSMDATA_TXIS;
            }
            else if(i2c->State == I2C_STATE_BUSY_RX)
            {
                i2c->Process = I2C_REREADDATA_RERX;
            }
            else
            {
            //It should not be here
            }
            break;
        case I2C_TRANSMDATA_TXIS:
            if (!LL_I2C_IsActiveFlag_TXE(i2c->Instance)) return;
            if (i2c->vSize > 0)
            {
                LL_I2C_TransmitData8(i2c->Instance, *(i2c->pBuffer)++);
                i2c->vSize--;
            }
            else
            {
                if (!LL_I2C_IsActiveFlag_TC(i2c->Instance)) return;
                LL_I2C_GenerateStopCondition(i2c->Instance);
                i2c->Process = I2C_DONE;
                i2c->State = I2C_STATE_READY;
                LL_I2C_DisableIT_TX(i2c->Instance);
                return;
            }
            break;
        case I2C_REREADDATA_RERX:
            if (!LL_I2C_IsActiveFlag_TXE(i2c->Instance)) return;
            LL_I2C_TransmitData8(i2c->Instance, (i2c->SlaveAddress) | 1);
            i2c->Process = I2C_READWAITING;
            break;
        case I2C_READWAITING:
            if (!LL_I2C_IsActiveFlag_ADDR(i2c->Instance)) return;
            LL_I2C_ClearFlag_ADDR(i2c->Instance);
            i2c->Process = I2C_READPROCESSING;
            break;
        case I2C_READPROCESSING:
            if (!LL_I2C_IsActiveFlag_RXNE(i2c->Instance)) return;
            if ((i2c->vSize) > 0){
                *(i2c->pBuffer)++ = LL_I2C_ReceiveData8(i2c->Instance);
                (i2c->vSize)--;
            }
            if ((i2c->vSize) > 0)
            {
                i2c->RegAddress += 1;
                i2c->Process = I2C_ADDRSENT_ADDR;
                return;
            }
            if ((i2c->vSize) == 0)
            {
                LL_I2C_GenerateStopCondition(i2c->Instance);
                LL_I2C_DisableIT_RX(i2c->Instance);
//                LL_I2C_DisableIT_EVT(i2c->Instance);
                i2c->Process = I2C_DONE;
                i2c->State = I2C_STATE_READY;
                return;
            }
            break;
        default:
            /* It should not be here */
            break;
    }
}



