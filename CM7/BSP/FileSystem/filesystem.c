/*
 * filesystem.c
 *
 *  Created on: Dec 18, 2024
 *      Author: CAO HIEU
 */
#include "filesystem.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
SDFS_StateTypedef SDFS_State = SDFS_READY;

void FS_ListFiles_path(void);

FATFS MMC1FatFs;  /* File system object for SD card logical drive */
FIL MyFile1;     /* File object */
char MMC1Path[4]; /* SD card logical drive path */

FATFS MMC2FatFs;  /* File system object for SD card logical drive */
FIL MyFile2;     /* File object */
char MMC2Path[4]; /* SD card logical drive path */


uint8_t workBuffer[FF_MAX_SS];
ALIGN_32BYTES(uint8_t rtext[96]);

uint8_t wtext[] = "This is FatFs running on CM7 core 2nd try"; /* File write buffer */

void SD_Lockin(void){
	LL_GPIO_SetOutputPin(GPIOE, SDMMC_CTRL_ENA_Pin);
	LL_GPIO_SetOutputPin(GPIOD, SDMMC2_ENA_Pin);
	LL_GPIO_ResetOutputPin(GPIOE, SDMMC_SEL2_Pin);



	LL_GPIO_ResetOutputPin(GPIOD, SDMMC1_ENA_Pin);
	LL_GPIO_SetOutputPin(GPIOE, SDMMC_SEL1_Pin);
	SDFS_State = SDFS_READY;
}


void SD_Release(void){
	LL_GPIO_ResetOutputPin(GPIOD, SDMMC2_ENA_Pin);
	LL_GPIO_ResetOutputPin(GPIOD, SDMMC2_ENA_Pin);
	LL_GPIO_SetOutputPin(GPIOE, SDMMC_SEL2_Pin);



	LL_GPIO_ResetOutputPin(GPIOD, SDMMC1_ENA_Pin);
	LL_GPIO_SetOutputPin(GPIOE, SDMMC_SEL1_Pin);
	SDFS_State = SDFS_RELEASE;
}

uint32_t file_size;

void FS_ListFiles_path(void)
{
  FRESULT res;
  DIR dir;
  FILINFO fno;
  char *path = MMC2Path;
  char buffer[384];
  snprintf(buffer, sizeof(buffer),
		  "\r\n[CM7]: Listing files in %s...\r\n",
           path);
  CDC_SendString(buffer);


  /* Open the root directory */
  res = f_opendir(&dir, path);
  if (res == FR_OK)
  {
    while (1)
    {
      res = f_readdir(&dir, &fno); /* Read a directory item */
      if (res != FR_OK || fno.fname[0] == 0)
      {
        break; /* Break on error or end of directory */
      }
      if (fno.fattrib & AM_DIR)
      {
        snprintf(buffer, sizeof(buffer),
        		"  [DIR]  %s\r\n", fno.fname);
        CDC_SendString(buffer);

      }
      else
      {
          snprintf(buffer, sizeof(buffer),
          		"  [FILE] %s\t%lu bytes\r\n", fno.fname, (unsigned long)fno.fsize);
          CDC_SendString(buffer);
      }
    }
    f_closedir(&dir);
  }
  else
  {
    snprintf(buffer, sizeof(buffer),
    		"Failed to open directory %s: %d\r\n", path, res);
    CDC_SendString(buffer);
  }
}

void Link_SDFS_Driver(void){
	if(FATFS_LinkDriver(&MMC2_Driver, MMC2Path) == 0)
	{
	//	printf("MMC Instance Base Address when done link drive: 0x%p ", (void*)hmmc1.Instance);
		  int ret2 = FR_OK;

		  ret2 = f_mount(&MMC2FatFs, (TCHAR const*)MMC2Path, 1);

		  if(ret2 != FR_OK)
		  {
			  /* FatFs Initialization Error */
			  CDC_SendString("Failed to mount\r\n");
//			  Error_Handler();
		  }
	}

}

int Cat_SDFS(const char *filename) {
    FIL file;
    FRESULT res;
    char buffer[384];
    ALIGN_32BYTES(uint8_t rtext[96]);
    UINT bytesread;

    // Mở file chỉ để đọc
    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "Failed to open file %s: %d\r\n", filename, res);
        CDC_SendString(buffer);
        return -1; // Lỗi mở file
    }
    CDC_SendString("\r\n");
    // Đọc nội dung file
    do {
        memset(rtext, 0, sizeof(rtext)); // Làm sạch buffer trước mỗi lần đọc
        res = f_read(&file, rtext, sizeof(rtext) - 1, &bytesread);
        if (res != FR_OK) {
            snprintf(buffer, sizeof(buffer), "Failed to read file %s: %d\r\n", filename, res);
            CDC_SendString(buffer);
            f_close(&file);
            return -1; // Lỗi đọc file
        }
        rtext[bytesread] = '\0'; // Đảm bảo buffer là chuỗi kết thúc
        CDC_SendString((char *)rtext);
    } while (bytesread > 0);

    CDC_SendString("\r\n");
    f_close(&file);
    return 0; // Thành công
}

int Nano_SDFS(const char *filename, const char *content) {
    FIL file;
    FRESULT res;
    char buffer[384];
    UINT byteswritten;

    // Mở hoặc tạo tệp, ghi vào cuối
    res = f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "Failed to open or create file %s: %d\r\n", filename, res);
        CDC_SendString(buffer);
        return -1; // Lỗi mở hoặc tạo file
    }

    // Ghi nội dung nếu có
    if (content && strlen(content) > 0) {
        res = f_write(&file, content, strlen(content), &byteswritten);
        if (res != FR_OK || byteswritten != strlen(content)) {
            snprintf(buffer, sizeof(buffer), "Failed to write to file %s: %d\r\n", filename, res);
            CDC_SendString(buffer);
            f_close(&file);
            return -1; // Lỗi ghi file
        }
        // Thêm xuống dòng vào cuối
        res = f_write(&file, "\r\n", 2, &byteswritten);
        if (res != FR_OK) {
            snprintf(buffer, sizeof(buffer), "Failed to write newline to file %s: %d\r\n", filename, res);
            CDC_SendString(buffer);
            f_close(&file);
            return -1; // Lỗi ghi xuống dòng
        }
    }

    f_close(&file);
    return 0; // Thành công
}

int Vim_SDFS(const char *filename, const char *content) {
    FIL file;
    FRESULT res;
    char buffer[384];
    UINT byteswritten;

    // Mở hoặc tạo tệp, ghi vào cuối
    res = f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE);
    if (res != FR_OK) {
        snprintf(buffer, sizeof(buffer), "Failed to open or create file %s: %d\r\n", filename, res);
        CDC_SendString(buffer);
        return -1; // Lỗi mở hoặc tạo file
    }

    // Ghi nội dung mới vào hàng mới
    if (content && strlen(content) > 0) {

        // Ghi nội dung mới
        res = f_write(&file, content, strlen(content), &byteswritten);
        if (res != FR_OK || byteswritten != strlen(content)) {
            snprintf(buffer, sizeof(buffer), "Failed to write content to file %s: %d\r\n", filename, res);
            CDC_SendString(buffer);
            f_close(&file);
            return -1; // Lỗi ghi nội dung
        }

        // Thêm xuống dòng trước khi ghi nội dung mới (trừ khi tệp đang rỗng)
        if (f_size(&file) > 0) {
            res = f_write(&file, "\r\n", 2, &byteswritten);
            if (res != FR_OK) {
                snprintf(buffer, sizeof(buffer), "Failed to write newline to file %s: %d\r\n", filename, res);
                CDC_SendString(buffer);
                f_close(&file);
                return -1; // Lỗi ghi xuống dòng
            }
        }

    }

    f_close(&file);
    return 0; // Thành công
}


//
//int NanoCat_SDFS(const char *filename, const char *content) {
//    FIL file;
//    FRESULT res;
//    char buffer[384];
//    uint32_t byteswritten, bytesread;
//    ALIGN_32BYTES(uint8_t rtext[96]);
////    // Mở hoặc tạo tệp
////    res = f_open(&file, filename, FA_OPEN_APPEND | FA_WRITE | FA_CREATE_ALWAYS);
////    if (res != FR_OK) {
////        snprintf(buffer, sizeof(buffer), "Failed to open or create file %s: %d\r\n", filename, res);
////        CDC_SendString(buffer);
////        return -1;
////    }
////
////    res = f_write(&file, wtext, sizeof(wtext), (void *)&byteswritten);
////
////    if((byteswritten == 0) || (res != FR_OK))
////    {
////    	            snprintf(buffer, sizeof(buffer), "Failed to write to file %s: %d\r\n", filename, res);
////    	            CDC_SendString(buffer);
////    }
////
////    f_close(&file);
//
//    if(f_open(&file, filename, FA_READ) != FR_OK)
//    {
//        snprintf(buffer, sizeof(buffer), "Failed to write to file %s: %d\r\n", filename, res);
//        CDC_SendString(buffer);
//    }
//
//
//    res = f_read(&file, rtext, sizeof(rtext), (void *)&bytesread);
//    if((bytesread == 0) || (res != FR_OK))
//    {
//    	    	            snprintf(buffer, sizeof(buffer), "Failed to write to file %s: %d\r\n", filename, res);
//    	    	            CDC_SendString(buffer);
//    }
//    /* Close the open text file */
//    f_close(&file);
//
//    snprintf(buffer, sizeof(buffer), "Data: %s\r\n", rtext);
//    CDC_SendString(buffer);
//
//    // Ghi nội dung nếu có
////    if (content && strlen(content) > 0) {
////        UINT written;
////        res = f_write(&file, content, strlen(content), &written);
////        if (res != FR_OK || written != strlen(content)) {
////            snprintf(buffer, sizeof(buffer), "Failed to write to file %s: %d\r\n", filename, res);
////            CDC_SendString(buffer);
////            f_close(&file);
////            return -1;
////        }
////        f_write(&file, "\r\n", 2, &written);
////    }
////    f_close(&file);
//
////    // Mở lại file để đọc
////    res = f_open(&file, filename, FA_READ);
////    if (res != FR_OK) {
////        snprintf(buffer, sizeof(buffer), "Failed to open file %s for reading: %d\r\n", filename, res);
////        CDC_SendString(buffer);
////        return -1;
////    }
////
////    // Đọc nội dung file và in ra
////    snprintf(buffer, sizeof(buffer), "Content of file %s:\r\n", filename);
////    CDC_SendString(buffer);
////
////    char fileContent[256];
////    UINT readBytes;
////    memset(fileContent, 0, sizeof(fileContent)); // Clear buffer
////
////    do {
////    	printf("zo!\r\n");
////        res = f_read(&file, fileContent, sizeof(fileContent) - 1, &readBytes);
////        if (res != FR_OK) {
////            snprintf(buffer, sizeof(buffer), "Failed to read file %s: %d\r\n", filename, res);
////            CDC_SendString(buffer);
////            break;
////        }
////        fileContent[readBytes] = '\0'; // Kết thúc chuỗi
////        CDC_SendString(fileContent);
////    } while (readBytes > 0);
////
////    f_close(&file);
//
//    return 0;
//}



//if((FATFS_LinkDriver(&MMC1_Driver, MMC1Path) == 0) && (FATFS_LinkDriver(&MMC2_Driver, MMC2Path) == 0))
//{
////	printf("MMC Instance Base Address when done link drive: 0x%p ", (void*)hmmc1.Instance);
//	  int ret1 = FR_OK;
//	  int ret2 = FR_OK;
//
//	  ret1 = f_mount(&MMC1FatFs, (TCHAR const*)MMC1Path, 1);
//
////	  FRESULT fr;
////	  FIL file;
////	  char filename[] = "textfile_F.txt";
////
////	  // Kiểm tra trên MMC1Path
////	  fr = f_open(&file, filename, FA_READ);
////	  if (fr == FR_OK) {
////	      // Tệp tồn tại
////	      f_close(&file);
////	      printf("File %s exists on MMC1Path.\n", filename);
////	  } else if (fr == FR_NO_FILE) {
////	      // Tệp không tồn tại
////	      printf("File %s does not exist on MMC1Path.\n", filename);
////	  } else {
////	      // Lỗi khác
////	      printf("An error occurred while checking the file: %d\n", fr);
////	  }
//
//
//	  ret2 = f_mount(&MMC2FatFs, (TCHAR const*)MMC2Path, 1);
//
//	  if((ret1 != FR_OK) || (ret2 != FR_OK))
//	  {
//	      /* FatFs Initialization Error */
//		  printf("Failed to mount\r\n");
//		  Error_Handler();
//	  }
