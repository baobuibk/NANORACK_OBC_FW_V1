/**
  ******************************************************************************
  * @file    FatFs/FatFs_Shared_Device/Common/Inc/mmc_diskio.h
  * @author  MCD Application Team
  * @brief   Header for mmc_diskio.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MMC_DISKIO_H
#define __MMC_DISKIO_H

/* Includes ------------------------------------------------------------------*/
#include "emmc_stm32h745zi.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

 #define DUAL_MMC       // Use 2 MMC at the same time
// #define ONLY_MMC1    // Use 1: MMC1
// #define ONLY_MMC2    // Use 1: MMC2


//extern const Diskio_drvTypeDef  MMC_Driver;

#if defined(DUAL_MMC) || defined(ONLY_MMC1)
extern const Diskio_drvTypeDef MMC1_Driver;
#endif

#if defined(DUAL_MMC) || defined(ONLY_MMC2)
extern const Diskio_drvTypeDef MMC2_Driver;
#endif

#if !defined(DUAL_MMC) && !defined(ONLY_MMC1) && !defined(ONLY_MMC2)
#warning "MMC haven't been selected yet! Define DUAL_MMC, ONLY_MMC1, or ONLY_MMC2."
#endif



#endif /* __MMC_DISKIO_H */


