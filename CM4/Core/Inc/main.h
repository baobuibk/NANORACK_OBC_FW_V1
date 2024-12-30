/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_ll_i2c.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_crs.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../BSP/System_IPC/ring_buffer.h"
#include "../Peripherials/I2C/i2c_it.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern volatile ringbuff_t* rb_cm4_to_cm7;
extern volatile ringbuff_t* rb_cm7_to_cm4;
extern UART_HandleTypeDef huart1;
extern LL_I2C_HandleTypeDef ll_i2c1;
extern LL_I2C_HandleTypeDef ll_i2c4;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void clear_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void set_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI6_ISO_CS_Pin LL_GPIO_PIN_3
#define SPI6_ISO_CS_GPIO_Port GPIOE
#define CAM_SPI_CS_Pin LL_GPIO_PIN_6
#define CAM_SPI_CS_GPIO_Port GPIOF
#define MCU_CTRL_SRC_ISO_Pin LL_GPIO_PIN_6
#define MCU_CTRL_SRC_ISO_GPIO_Port GPIOA
#define SERIAL_ENA_Pin LL_GPIO_PIN_14
#define SERIAL_ENA_GPIO_Port GPIOD
#define USB_CTRL_Pin LL_GPIO_PIN_7
#define USB_CTRL_GPIO_Port GPIOC
#define WD_DONE_Pin LL_GPIO_PIN_5
#define WD_DONE_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
void I2C4_ReInit(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
