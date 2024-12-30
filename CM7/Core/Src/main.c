/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "scheduler.h"

#include "../Task/LED_Blink/blink_led_debug.h"
#include "../../BSP/CMDLine_M7/command.h"
#include "../Task/rtc_update/rtc_update.h"
#include "filesystem.h"
#include "../Task/dualcore_cmd/dualcore_cmd.h"
#include "usbd_cdc_if.h"
#include "../Task/Fake_EXP/fake_exp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

MMC_HandleTypeDef hmmc1;
MMC_HandleTypeDef hmmc2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile ringbuff_t* rb_cm4_to_cm7 = (void *)BUFF_CM4_TO_CM7_ADDR;
volatile ringbuff_t* rb_cm7_to_cm4 = (void *)BUFF_CM7_TO_CM4_ADDR;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SDMMC1_MMC_Init(void);
static void MX_SDMMC2_MMC_Init(void);
static void MX_SPI4_Init(void);
static void MX_SPI5_Init(void);
/* USER CODE BEGIN PFP */
#define CM7_FILE   "activities.log"
#define PRINTF2USART2 int __io_putchar(int data)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


//static void FS_FileOperations(void);
//static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
//  int32_t timeout;
//  FRESULT res;
  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */
ringbuff_init(rb_cm7_to_cm4, (void *)BUFFDATA_CM7_TO_CM4_ADDR, BUFFDATA_CM7_TO_CM4_LEN);
ringbuff_init(rb_cm4_to_cm7, (void *)BUFFDATA_CM4_TO_CM7_ADDR, BUFFDATA_CM4_TO_CM7_LEN);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	//  MX_SDMMC1_MMC_Init();
	SD_Lockin();

	MX_SDMMC2_MMC_Init();

	MX_USB_DEVICE_Init();
	MX_SPI4_Init();
	MX_SPI5_Init();



	/* USER CODE BEGIN 2 */
	Link_SDFS_Driver();


//  FS_ListFiles_path();
//  LL_GPIO_SetOutputPin(GPIOE, SDMMC_CTRL_ENA_Pin);
//
//  LL_GPIO_SetOutputPin(GPIOD, SDMMC1_ENA_Pin);
//  LL_GPIO_ResetOutputPin(GPIOE, SDMMC_SEL1_Pin);
//
//  LL_GPIO_SetOutputPin(GPIOD, SDMMC2_ENA_Pin);
//  LL_GPIO_ResetOutputPin(GPIOE, SDMMC_SEL2_Pin);

     log_struct_t log = {
         .date = "2024-02-22",
         .time = "23:40:03",
         .actTime = "0:05:03",
         .level = LOG_NOTICE,
         .core = "M7",
         .function = "main",
         .line = 172,
         .message = "Start up"
     };
     char log_output[256];
     log_create(log, log_output);
     if(SDFS_State == SDFS_READY){
		 if (Vim_SDFS(CM7_FILE, log_output) == 0) {
//			 CDC_SendString("\r\nContent written successfully.\r\n");
		 } else {
			 CDC_SendString("\r\nFailed to write content.\r\n");
		 }
     }


//printf("eMMC init successfully!\r\n");




//printf("On eMMC 2->1 init!\r\n");
//MX_SDMMC2_MMC_Init();
//printf("Done 2 init!\r\n");

//MX_SDMMC1_MMC_Init();
//printf("Done 1 init!\r\n");
/* USER CODE BEGIN 2 */
//printf("eMMC init successfully!\r\n");


//  LL_GPIO_SetOutputPin(GPIOD, SDMMC1_ENA_Pin);
//  LL_GPIO_SetOutputPin(GPIOE, SDMMC_CTRL_ENA_Pin);
//  LL_GPIO_ResetOutputPin(GPIOE, SDMMC_SEL1_Pin);
//
//  LL_GPIO_SetOutputPin(GPIOD, SDMMC2_ENA_Pin);
//  LL_GPIO_ResetOutputPin(GPIOE, SDMMC_SEL2_Pin);
/*-1- Link the micro SD disk I/O driver*/

////

//
//
//
//
//
//	  printf("Do SDMMC1\r\n");
//	  FS_ListFiles_path(MMC1Path);
//	  printf("Done SDMMC1\r\n");
//
//	  printf("Do SDMMC2\r\n");
//	  /* List all files in the root directory */
//	  FS_ListFiles_path(MMC2Path);
//	  printf("Done SDMMC2\r\n");
//
//}


/*

*/
//  if (HAL_SD_Init(&hsd1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//
//  if (HAL_MMC_ConfigWideBusOperation(&hmmc1, SDMMC_BUS_WIDE_4B) != HAL_OK)
//  {
//	  Error_Handler();
//  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SCH_Initialize();


  command_create_task();
  RTC_create_task();
  status_led_create_task();

  cmdline_init();

  DUALCORECMD_create_task();
  FAKEEXP_create_task();

  SCH_StartSchedular();


while (1)
{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	SCH_HandleScheduledTask();

//    size_t len = 0;
//    void* addr;
////	printf("Heart beat!\r\n  ");
////	HAL_Delay(3000);
////    printf("BeforeLength: %d\r\n", len);
//	while ((len = ringbuff_get_linear_block_read_length(rb_cm4_to_cm7)) > 0) {
//	    addr = ringbuff_get_linear_block_read_address(rb_cm4_to_cm7);
//	    /* Transmit data */
////	    printf("GOT1!\r\n");
////	    printf("GOT2!\r\n");
////	    printf("Length: %d\r\n", len);
//	    HAL_UART_Transmit(&huart2, addr, len, 1000);
////	    printf("GOT3!\r\n");
//	    /* Mark buffer as read */
//	    ringbuff_skip(rb_cm4_to_cm7, len);
////	    printf("-------------->>>>>Length: %d\r\n", len);
//	}
//	  printf("AfterLength: %d\r\n", len);
//
//	  counter_log++;
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_HSI48_Enable();

   /* Wait till HSI48 is ready */
  while(LL_RCC_HSI48_IsReady() != 1)
  {

  }
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1Q_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(5);
  LL_RCC_PLL1_SetN(96);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(8);
  LL_RCC_PLL1_SetR(4);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {

  }
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);
  LL_SetSystemCoreClock(240000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  LL_RCC_PLL2Q_Enable();
  LL_RCC_PLL2_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_4_8);
  LL_RCC_PLL2_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL2_SetM(5);
  LL_RCC_PLL2_SetN(48);
  LL_RCC_PLL2_SetP(2);
  LL_RCC_PLL2_SetQ(30);
  LL_RCC_PLL2_SetR(4);
  LL_RCC_PLL2_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL2_IsReady() != 1)
  {
  }

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_MMC_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hmmc1.Instance = SDMMC1;
  hmmc1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hmmc1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hmmc1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hmmc1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hmmc1.Init.ClockDiv = 5;
  if (HAL_MMC_Init(&hmmc1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */
//  printf("MMC Instance Base Address when done init: 0x%p ", (void*)hmmc1.Instance);

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SDMMC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC2_MMC_Init(void)
{

  /* USER CODE BEGIN SDMMC2_Init 0 */

  /* USER CODE END SDMMC2_Init 0 */

  /* USER CODE BEGIN SDMMC2_Init 1 */

  /* USER CODE END SDMMC2_Init 1 */
  hmmc2.Instance = SDMMC2;
  hmmc2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hmmc2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hmmc2.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hmmc2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hmmc2.Init.ClockDiv = 5;
  if (HAL_MMC_Init(&hmmc2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC2_Init 2 */

  /* USER CODE END SDMMC2_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetSPIClockSource(LL_RCC_SPI45_CLKSOURCE_PLL2Q);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI4);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
  /**SPI4 GPIO Configuration
  PE5   ------> SPI4_MISO
  PE6   ------> SPI4_MOSI
  PE12   ------> SPI4_SCK
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_12;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 0x0;
  LL_SPI_Init(SPI4, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI4, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_SetFIFOThreshold(SPI4, LL_SPI_FIFO_TH_01DATA);
  LL_SPI_EnableNSSPulseMgt(SPI4);
  /* USER CODE BEGIN SPI4_Init 2 */
  LL_SPI_Enable(SPI4);
  LL_SPI_StartMasterTransfer(SPI4);
  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetSPIClockSource(LL_RCC_SPI45_CLKSOURCE_PLL2Q);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
  /**SPI5 GPIO Configuration
  PF7   ------> SPI5_SCK
  PF8   ------> SPI5_MISO
  PF9   ------> SPI5_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 0x0;
  LL_SPI_Init(SPI5, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI5, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_SetFIFOThreshold(SPI5, LL_SPI_FIFO_TH_01DATA);
  LL_SPI_EnableNSSPulseMgt(SPI5);
  /* USER CODE BEGIN SPI5_Init 2 */
  LL_SPI_Enable(SPI5);
  LL_SPI_StartMasterTransfer(SPI5);
  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);

  /**/
  LL_GPIO_ResetOutputPin(GPIOE, CS_RTC_Pin|SDMMC_CTRL_ENA_Pin|SDMMC_SEL1_Pin|SDMMC_SEL2_Pin);

  /**/
  LL_GPIO_ResetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, SDMMC2_ENA_Pin|SDMMC1_ENA_Pin);

  /**/
  LL_GPIO_SetOutputPin(HUB_RESET_GPIO_Port, HUB_RESET_Pin);

  /**/
  GPIO_InitStruct.Pin = CS_RTC_Pin|SDMMC_CTRL_ENA_Pin|SDMMC_SEL1_Pin|SDMMC_SEL2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED_DEBUG_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED_DEBUG_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SDMMC2_ENA_Pin|SDMMC1_ENA_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = HUB_RESET_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(HUB_RESET_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
PRINTF2USART2{
	HAL_UART_Transmit(&huart2, (uint8_t *) &data, 1, 0xFFFF);
	return data;
}

//static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    if (*pBuffer1 != *pBuffer2)
//    {
//      return 1;
//    }
//
//    pBuffer1++;
//    pBuffer2++;
//  }
//  return 0;
//}



//static void FS_ListFiles(void)
//{
//  FRESULT res;
//  DIR dir;
//  FILINFO fno;
//
//  printf("[CM7]:\tListing files...\r\n");
//
//  /* Open the root directory */
//  res = f_opendir(&dir, "/");
//  if (res == FR_OK)
//  {
//    while (1)
//    {
//      res = f_readdir(&dir, &fno); /* Read a directory item */
//      if (res != FR_OK || fno.fname[0] == 0)
//      {
//        break; /* Break on error or end of directory */
//      }
//      if (fno.fattrib & AM_DIR)
//      {
//        printf("  [DIR]  %s\r\n", fno.fname); /* It is a directory */
//      }
//      else
//      {
//    	  printf("  [FILE] %s\t%lu bytes\r\n", fno.fname, (unsigned long)fno.fsize);
//      }
//    }
//    f_closedir(&dir);
//  }
//  else
//  {
//    printf("Failed to open directory: %d\r\n", res);
//  }
//}


//static void FS_FileOperations(void)
//{
//  FRESULT res;
//  uint32_t byteswritten, bytesread;
//
//  /* Register the file system object to the FatFs module */
//  if (f_mount(&MMCFatFs, (TCHAR const*)MMCPath, 0) != FR_OK)
//  {
//    printf("Failed to mount\r\n");
//    goto error;
//  }
//
//  /* List all files in the root directory */
//  FS_ListFiles();
//
//  /* Check if file exists */
//  if (f_stat(CM7_FILE, NULL) == FR_NO_FILE)
//  {
//    printf("[CM7]:\tFile '%s' does not exist. Creating...\r\n", CM7_FILE);
//  }
//
//  /* Open the file "CM7.TXT" in write mode */
//  res = f_open(&MyFile, CM7_FILE, FA_CREATE_ALWAYS | FA_WRITE);
//  if (res != FR_OK)
//  {
//    printf("Failed to open\r\n");
//    goto error;
//  }
//
//  printf("[CM7]:\tOpening '%s' for writing...\r\n", CM7_FILE);
//
//  /* Write data to the text file */
//  res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
//
//  if ((byteswritten == 0) || (res != FR_OK))
//  {
//    f_close(&MyFile);
//    goto error;
//  }
//
//  printf("[CM7]:\t'%s' '%lu' bytes written...\r\n", CM7_FILE, byteswritten);
//
//  /* Close the open text file */
//  f_close(&MyFile);
//
//  /* Open the text file object with read access */
//  if (f_open(&MyFile, CM7_FILE, FA_READ) != FR_OK)
//  {
//    goto error;
//  }
//
//  printf("[CM7]:\tReading '%s' content...\r\n", CM7_FILE);
//
//  /* Read data from the text file */
//  res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
//  if ((bytesread == 0) || (res != FR_OK))
//  {
//    f_close(&MyFile);
//    goto error;
//  }
//
//  /* Close the open text file */
//  f_close(&MyFile);
//
//  /* Compare read data with the expected data */
//  if (Buffercmp(rtext, (uint8_t *)wtext, byteswritten) != 0)
//  {
//    goto error;
//  }
//  else
//  {
//    printf("[CM7]:\t%s ==> '>  %s <'\r\n", CM7_FILE, rtext);
//  }
//
//error:
//  Error_Handler();
//}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{

  /* Disables the MPU */
  LL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x87, 0x0, LL_MPU_REGION_SIZE_4GB|LL_MPU_TEX_LEVEL0|LL_MPU_REGION_NO_ACCESS|LL_MPU_INSTRUCTION_ACCESS_DISABLE|LL_MPU_ACCESS_SHAREABLE|LL_MPU_ACCESS_NOT_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
  /* Enables the MPU */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

	printf("Error_Handler!\r\n");
	while(1){
		HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, 0);
		HAL_Delay(50);
	}

  __disable_irq();
 // HAL_GPIO_WritePin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin, 1);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
