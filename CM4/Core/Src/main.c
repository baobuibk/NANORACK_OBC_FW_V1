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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "scheduler.h"
#include "pet_watchdog_m4.h"
#include "blink_led_debug.h"


#include "log.h"


#include "i2c_mcp23017.h"
#include "spi_ads8327.h"
#include "../Task/dualcore_cmd/dualcore_cmd.h"

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

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART7_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C4_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI6_Init(void);
/* USER CODE BEGIN PFP */
#define PRINTF2USART1 int __io_putchar(int data)

volatile ringbuff_t* rb_cm4_to_cm7 = (void *)BUFF_CM4_TO_CM7_ADDR;
volatile ringbuff_t* rb_cm7_to_cm4 = (void *)BUFF_CM7_TO_CM4_ADDR;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
LL_I2C_HandleTypeDef ll_i2c1;
LL_I2C_HandleTypeDef ll_i2c4;
void DS3231_Read(uint8_t *data);
void DS3231_Write(uint16_t reg_address, uint8_t length, uint8_t *data);
void SendByte(uint8_t data);
void SendString(const char *str);
//
//FDCAN_TxHeaderTypeDef   TxHeader;
//FDCAN_RxHeaderTypeDef   RxHeader;
//uint8_t               TxData[8];
//uint8_t               RxData[8];
//int indx = 0;
//
//void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
//{
//  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
//  {
//    /* Retreive Rx messages from RX FIFO0 */
//    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
//    {
//    /* Reception Error */
//    Error_Handler();
//    }
//    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
//    {
//      /* Notification Error */
//      Error_Handler();
//    }
//  }
//}

void I2C4_ReInit(void) {
    LL_I2C_Disable(I2C4);
    MX_I2C4_Init();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  /*
  Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
  perform system initialization (system clock config, external memory configuration.. )
  */
  HAL_PWREx_ClearPendingEvent();
  HAL_PWREx_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));

/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART7_Init();
  MX_USART1_UART_Init();
  MX_I2C4_Init();
  MX_I2C1_Init();
  MX_SPI6_Init();
  /* USER CODE BEGIN 2 */

  ll_i2c1.Instance = I2C1;
  ll_i2c1.State = I2C_STATE_READY;
  ll_i2c1.Process = I2C_DONE;

  ll_i2c4.Instance = I2C4;
  ll_i2c4.State = I2C_STATE_READY;
  ll_i2c4.Process = I2C_DONE;








//  HAL_Delay(500);
//  LL_GPIO_SetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
//  HAL_Delay(500);
//  LL_GPIO_ResetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
//  HAL_Delay(500);

  LL_GPIO_SetOutputPin(CAM_SPI_CS_GPIO_Port, CAM_SPI_CS_Pin);
  LL_GPIO_SetOutputPin(USB_CTRL_GPIO_Port, USB_CTRL_Pin);
  LL_GPIO_SetOutputPin(SERIAL_ENA_GPIO_Port, SERIAL_ENA_Pin);
  LL_GPIO_SetOutputPin(MCU_CTRL_SRC_ISO_GPIO_Port, MCU_CTRL_SRC_ISO_Pin);
//  while(1){
//	  LL_GPIO_SetOutputPin(SERIAL_ENA_GPIO_Port, SERIAL_ENA_Pin);
////	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
////	  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
//	  HAL_Delay(50);
//	  LL_GPIO_ResetOutputPin(SERIAL_ENA_GPIO_Port, SERIAL_ENA_Pin);
////	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
////	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
//	  HAL_Delay(50);
//  }


  SCH_Initialize();

  Watchdog_M4_create_task();
  status_led_create_task();
  DUALCORECMD_create_task();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  SCH_StartSchedular();

//  int ret = Status_OK;

//  ret = I2C_Mem_Write_IT(&ll_i2c1, 0x68, 0x00, abc, 1);
//  if(ret == Status_BUSY){
//	  printf("BUSY\r\n");
//  }else{
//	  printf("I2C_OK\r\n");
//  }
//  uint8_t datalink[2];
//  datalink[0] = ((2 / 10) << 4) | (2 % 10);
//  datalink[1] = ((2 / 10) << 4) | (2 % 10);
////  DS3231_Write(1, 1 ,datalink);
//  ret = I2C_Write(&ll_i2c4, 0x68, 0, datalink, 2);
//  if(ret == Status_OK){
//	  printf("Write OK \r\n");
//  }else if(ret == Status_TIMEOUT ){
//	  printf("Write Timeout \r\n");
//  } else{
//	  printf("Write Error \r\n");
//  }
//  uint8_t abc[3];
  printf("Init!\r\n");
  HAL_Delay(1000);

  while (!ringbuff_is_ready(rb_cm7_to_cm4)) {printf("CM7 to CM4 not ready"); HAL_Delay(50);}
  while (!ringbuff_is_ready(rb_cm4_to_cm7)) {printf("CM4 to CM7 not ready"); HAL_Delay(50);}

  printf("OK CM4!\r\n");
  ringbuff_write(rb_cm4_to_cm7, "[CM4] Core ready\r\n", 18);
//  uint32_t i = 0, time, t1, t2;
//  time = t1 = t2 = HAL_GetTick();

  int retk = Status_OK;
//  retk = MCP23017_Init();
  if(retk == Status_OK){
	  printf("I2C Init OK! \r\n");
  }else{
	  printf("I2C Init Error! \r\n");
  }

    printf("Let's start\r\n");


//  while(1){
//	  HAL_Delay(50);
//
//	  ADS8327_Init();
//	  HAL_Delay(1000);

//	  ADS8327_Init();
//	  HAL_Delay(1000);
//
//	  ADS8327_Init();
//	  HAL_Delay(1000);
//
//	  ADS8327_RegWrite16(WRITE_CFR, 0x0EFD);
//
//	  printf("write!!!\r\n");
//	  ADS8327_RegWrite4(DEFAULT_MODE);
//	  SPI_LL_Write(SPI6, 0xC0);
//	  SPI_LL_Write(SPI6, 0xFF);
//	  printf("write!!!\r\n");
//	  HAL_Delay(1000);
//	  ADS8327_Init();
//	  HAL_Delay(1000);
//
//
//  }






  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  SCH_HandleScheduledTask();



//	  uint16_t adc_value = 0;
//	  ADS8327_Read(&adc_value);
//	  printf("adc value = %d \r\n", adc_value);
//	  printf("Hartbeat!\r\n");
//	  ringbuff_write(rb_cm4_to_cm7, "[CM4] Core ready\r\n", 18);
//	  char abc[]= "Helloworld\r\n";
//	  CDC_Transmit_FS((uint8_t*)abc, strlen("Helloworld\r\n"));
//	  HAL_Delay(3000);
//	  retk = MCP23017_DigitalWrite(MCP23017_GPB0_Pin, GPIO_PIN_SET);
//	  if(retk == Status_OK){
//		  printf("I2C GPIO DONE 1 \r\n");
//	  }else{
//		  printf("I2C GPIO FAIL 1\r\n");
//	  }
//	  HAL_Delay(500);
//
//	  retk = MCP23017_DigitalWrite(MCP23017_GPB0_Pin, GPIO_PIN_RESET);
//	  if(retk == Status_OK){
//		  printf("I2C GPIO DONE 2\r\n");
//	  }else{
//		  printf("I2C GPIO FAIL 2\r\n");
//	  }
//	  HAL_Delay(500);
//
//	  uint8_t abc;
//	  retk = MCP23017_DigitalRead(MCP23017_GPB1_Pin, &abc);
//	  printf("Data: %d\r\n", abc);

//	  for (int i=0; i<8; i++)
//	  {
//	   TxData[i] = indx++;
//	  }
//	  int ret = HAL_OK;
//	  ret = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData);
//	  if (ret!= HAL_OK)
//	  {
//		  printf("Error: %d\r\n", ret);
//		  printf("ErrorCODE: %ld \r\n", hfdcan1.ErrorCode);
//		  Error_Handler();
//
//	  }
//	  printf("Trans CAN!\r\n");

//	  while(1){
//		  LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_1);
//		  HAL_Delay(10);
//		  LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_1);
//		  HAL_Delay(10);
//	  }
//


//	  ret = I2C_Read(&ll_i2c4, 0x68, 0, abc, 3);
//	  if(ret == Status_OK){
//		  printf("Read OK \r\n");
//	  }else if(ret == Status_TIMEOUT ){
//		  printf("Read Timeout \r\n");
//	  } else{
//		  printf("Read Error \r\n");
//	  }
//	  printf("Time ssmmhh: %d:%d:%d\r\n", abc[0], abc[1], abc[2]);
//	  HAL_Delay(1000);
//	  SCH_HandleScheduledTask();
//	  HAL_Delay(1000);
//	  uint8_t a = 1;
//	  char buffer[5];
//	  while(1){
//		  sprintf(buffer, "%d", a);
//		  SendString(buffer);
//		  a++;
//
//		  if(a>100){
//			  a = 1;
//			  HAL_Delay(1000);
//		  }
//
//	  }


//      size_t len;
//      time = HAL_GetTick();

//      if (time - t1 >= 3000) {
//    	  printf("Got here!\r\n");
//          t1 = time;
//          char c = '0' + (++i % 10);
//
//          /* Write to buffer from CPU2 to CPU1 */
//          ringbuff_write(rb_cm4_to_cm7, "[CM4] Number: ", 14);
//          ringbuff_write(rb_cm4_to_cm7, &c, 1);
//          ringbuff_write(rb_cm4_to_cm7, "\r\n", 2);
//          printf("Sent!\r\n");
//      }
//      if (time - t1 >= 3000) {
//          printf("Got here!\r\n");
//          t1 = time;
//
//          char c = '0' + (++i % 10);
//          uint32_t hours = (time / 3600000) % 24;
//          uint32_t minutes = (time / 60000) % 60;
//          uint32_t seconds = (time / 1000) % 60;
//
//          char json_buffer[256];
//
//          uint8_t abc[1];
//          abc[0] = 0;
//          int ret;
//          ret = I2C_Read(&ll_i2c4, 0x12 , 0x01, abc, 1);
//          printf("Error code: %d\r\n", ret);
//          printf("I2C: %d\r\n", abc[0]);
//
//          snprintf(json_buffer, sizeof(json_buffer),
//              "{\r\n"
//              " \"Data\": {\r\n"
//              "  \"Core\": \"M4\",\r\n"
//              "  \"Date\": \"2024-02-22\",\r\n"
//              "  \"Time\": \"23:40:03\",\r\n"
//              "  \"Act\": \"%02lu:%02lu:%02lu\",\r\n"
//              "  \"Level\": \"NOTICE\",\r\n"
//              "  \"Message\": \"[CM4] Number: %c, Code: %d , Data: %d\"\r\n"
//              " },\r\n"
//              " \"Status\": {\r\n"
//              "  \"OK\"\r\n"
//              " }\r\n"
//              "}\r\n",
//              hours, minutes, seconds, c, ret, abc[0]);
//
//          ringbuff_write(rb_cm4_to_cm7, json_buffer, strlen(json_buffer));
////          SendByte(0xCA);
////
////          // Gửi chuỗi "InitCAN!\r\n"
////          SendString("InitCAN!\r\n");
////
////          // Gửi 0xEF
////          SendByte(0xEF);
//
//
//          rtc_sync_ll();
//
//
//          LL_GPIO_TogglePin(SPI6_ISO_CS_GPIO_Port, SPI6_ISO_CS_Pin);
//      }



//      while ((len = ringbuff_get_linear_block_read_length(rb_cm7_to_cm4)) > 0) {
//
//    	   void* addr = ringbuff_get_linear_block_read_address(rb_cm7_to_cm4);
//    	   (void)addr;
//          /*
//           * `addr` holds pointer to beginning of data array
//           * which can be used directly in linear form.
//           *
//           * Its length is `len` bytes
//           */
//          /* Process data here */
//
//          /* Mark buffer as read to allow other writes from CPU1 */
//          ringbuff_skip(rb_cm7_to_cm4, len);
//      }
//      HAL_Delay(10);

  }
  /* USER CODE END 3 */
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetI2CClockSource(LL_RCC_I2C123_CLKSOURCE_PCLK1);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x40E02E3A;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  /* USER CODE BEGIN I2C1_Init 2 */
  LL_I2C_Enable(I2C1);

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetI2CClockSource(LL_RCC_I2C4_CLKSOURCE_PCLK4);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
  /**I2C4 GPIO Configuration
  PF14   ------> I2C4_SCL
  PF15   ------> I2C4_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_I2C4);

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C4);
  LL_I2C_SetOwnAddress2(I2C4, 0, LL_I2C_OWNADDRESS2_NOMASK);
  LL_I2C_DisableOwnAddress2(I2C4);
  LL_I2C_DisableGeneralCall(I2C4);
  LL_I2C_EnableClockStretching(I2C4);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x40E32E37;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C4, &I2C_InitStruct);
  /* USER CODE BEGIN I2C4_Init 2 */
  LL_I2C_Enable(I2C4);

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief SPI6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI6_Init(void)
{

  /* USER CODE BEGIN SPI6_Init 0 */

  /* USER CODE END SPI6_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetSPIClockSource(LL_RCC_SPI6_CLKSOURCE_PCLK4);

  /* Peripheral clock enable */
  LL_APB4_GRP1_EnableClock(LL_APB4_GRP1_PERIPH_SPI6);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
  /**SPI6 GPIO Configuration
  PG12   ------> SPI6_MISO
  PG13   ------> SPI6_SCK
  PG14   ------> SPI6_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI6_Init 1 */

  /* USER CODE END SPI6_Init 1 */
  /* SPI6 parameter configuration*/
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
  LL_SPI_Init(SPI6, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI6, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_SetFIFOThreshold(SPI6, LL_SPI_FIFO_TH_01DATA);
  LL_SPI_EnableNSSPulseMgt(SPI6);
  /* USER CODE BEGIN SPI6_Init 2 */
  LL_SPI_Enable(SPI6);
  LL_SPI_StartMasterTransfer(SPI6);
  /* USER CODE END SPI6_Init 2 */

}

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  LL_USART_InitTypeDef UART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART234578_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART7);

  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
  /**UART7 GPIO Configuration
  PE7   ------> UART7_RX
  PE8   ------> UART7_TX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7|LL_GPIO_PIN_8;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* UART7 interrupt Init */
  NVIC_SetPriority(UART7_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART7_IRQn);

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
  UART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  UART_InitStruct.BaudRate = 1000000;
  UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  UART_InitStruct.Parity = LL_USART_PARITY_NONE;
  UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART7, &UART_InitStruct);
  LL_USART_DisableFIFO(UART7);
  LL_USART_SetTXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(UART7, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_ConfigAsyncMode(UART7);

  /* USER CODE BEGIN WKUPType UART7 */

  /* USER CODE END WKUPType UART7 */

  LL_USART_Enable(UART7);

  /* Polling UART7 initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(UART7))) || (!(LL_USART_IsActiveFlag_REACK(UART7))))
  {
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOD);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(SPI6_ISO_CS_GPIO_Port, SPI6_ISO_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(CAM_SPI_CS_GPIO_Port, CAM_SPI_CS_Pin);

  /**/
  LL_GPIO_ResetOutputPin(MCU_CTRL_SRC_ISO_GPIO_Port, MCU_CTRL_SRC_ISO_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, SERIAL_ENA_Pin|LL_GPIO_PIN_1|WD_DONE_Pin);

  /**/
  LL_GPIO_ResetOutputPin(USB_CTRL_GPIO_Port, USB_CTRL_Pin);

  /**/
  GPIO_InitStruct.Pin = SPI6_ISO_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(SPI6_ISO_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = CAM_SPI_CS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(CAM_SPI_CS_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = MCU_CTRL_SRC_ISO_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(MCU_CTRL_SRC_ISO_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = SERIAL_ENA_Pin|LL_GPIO_PIN_1|WD_DONE_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = USB_CTRL_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(USB_CTRL_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void set_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask) {
    LL_GPIO_SetOutputPin(GPIOx, PinMask);
}

void clear_pin(GPIO_TypeDef *GPIOx, uint32_t PinMask) {
    LL_GPIO_ResetOutputPin(GPIOx, PinMask);
}

PRINTF2USART1{
	HAL_UART_Transmit(&huart1, (uint8_t *) &data, 1, 0xFFFF);
//	while (!LL_USART_IsActiveFlag_TXE(USART1));
//	LL_USART_TransmitData8(USART1, data);
//	while (!LL_USART_IsActiveFlag_TC(USART1));
	return data;
}

#define I2Cx I2C1
#define DS3231_ADDRESS 0x68
#define I2C_TIMEOUT 50000

void DS3231_Read(uint8_t *data)
{
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx));

    LL_I2C_HandleTransfer(I2Cx, (DS3231_ADDRESS << 1), LL_I2C_ADDRSLAVE_7BIT, 1,LL_I2C_MODE_SOFTEND,LL_I2C_GENERATE_START_WRITE);

    while(!(LL_I2C_IsActiveFlag_TXIS(I2Cx)));
    LL_I2C_TransmitData8(I2Cx, 0);
    printf("Here4!\r\n");
    //while(!(LL_I2C_IsActiveFlag_TXIS(I2Cx)));
    while(LL_I2C_IsActiveFlag_TXE(I2Cx)==RESET);
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    printf("Here5!\r\n");

//    while(!(LL_I2C_IsActiveFlag_STOP(I2Cx)));

//    LL_I2C_GenerateStartCondition(I2Cx);
//    timeout = I2C_TIMEOUT;
//    while(!LL_I2C_IsActiveFlag_SB(I2Cx) && timeout--);
//    if (timeout == 0) return data;
    LL_I2C_HandleTransfer(I2Cx, 0xD0, ((DS3231_ADDRESS << 1) | 1), 3, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);

    for(int i = 0; i < 3; i++){
        while(LL_I2C_IsActiveFlag_RXNE(I2Cx)==0);
        data[i]=LL_I2C_ReceiveData8(I2Cx);
    }


//    LL_I2C_TransmitData8(I2Cx, DS3231_ADDRESS);
    while(!(LL_I2C_IsActiveFlag_STOP(I2Cx)));
    return;
}

void DS3231_Write(uint16_t reg_address, uint8_t length, uint8_t *data)
{
	while(LL_I2C_IsActiveFlag_BUSY(I2Cx));

    LL_I2C_HandleTransfer(I2Cx, (DS3231_ADDRESS << 1), LL_I2C_ADDRSLAVE_7BIT, 2+1 ,LL_I2C_MODE_AUTOEND,LL_I2C_GENERATE_START_WRITE );
    printf("Here1!\r\n");
    while(!(LL_I2C_IsActiveFlag_TXIS(I2Cx)));
    LL_I2C_TransmitData8(I2Cx, 0);

    for(uint8_t i = 0; i < length; i++)
    {
        // check if the TXIS flag is set
        while(!(LL_I2C_IsActiveFlag_TXIS(I2Cx)));

        // Write in the DR register the data to be sent
        LL_I2C_TransmitData8(I2Cx, data[i]);

        while(!(LL_I2C_IsActiveFlag_TXIS(I2Cx)));

        // Write in the DR register the data to be sent
        LL_I2C_TransmitData8(I2Cx, ((2 / 10) << 4) | (2 % 10));
    }

   // while(!(LL_I2C_IsActiveFlag_STOP(I2Cx)));


}

void SendByte(uint8_t data) {
    // �?ợi cho đến khi UART7 sẵn sàng để truy�?n
    while (!LL_USART_IsActiveFlag_TXE(UART7));
    // Gửi byte dữ liệu
    LL_USART_TransmitData8(UART7, data);
    // �?ợi cho đến khi byte được truy�?n xong
    while (!LL_USART_IsActiveFlag_TC(UART7));
}

void SendString(const char *str) {
    while (*str) {
        SendByte((uint8_t)(*str)); // Gửi từng byte trong chuỗi
        str++;
    }
}

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{

  /* Disables the MPU */
  LL_MPU_Disable();
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
	printf("Error HANDLER!\r\n");
	while(1){
//	LL_GPIO_SetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
//	HAL_Delay(50);
//	LL_GPIO_ResetOutputPin(LED_DEBUG_GPIO_Port, LED_DEBUG_Pin);
//	HAL_Delay(50);
	}
  __disable_irq();
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
