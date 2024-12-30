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
#include "ff_gen_drv.h"
#include "mmc_diskio.h"
#include "log.h"
#include "ring_buffer.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern volatile ringbuff_t* rb_cm4_to_cm7;
extern volatile ringbuff_t* rb_cm7_to_cm4;
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_RTC_Pin LL_GPIO_PIN_4
#define CS_RTC_GPIO_Port GPIOE
#define LED_DEBUG_Pin LL_GPIO_PIN_10
#define LED_DEBUG_GPIO_Port GPIOF
#define SDMMC_CTRL_ENA_Pin LL_GPIO_PIN_13
#define SDMMC_CTRL_ENA_GPIO_Port GPIOE
#define SDMMC_SEL1_Pin LL_GPIO_PIN_14
#define SDMMC_SEL1_GPIO_Port GPIOE
#define SDMMC_SEL2_Pin LL_GPIO_PIN_15
#define SDMMC_SEL2_GPIO_Port GPIOE
#define SDMMC2_ENA_Pin LL_GPIO_PIN_10
#define SDMMC2_ENA_GPIO_Port GPIOD
#define SDMMC1_ENA_Pin LL_GPIO_PIN_4
#define SDMMC1_ENA_GPIO_Port GPIOD
#define HUB_RESET_Pin LL_GPIO_PIN_9
#define HUB_RESET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
