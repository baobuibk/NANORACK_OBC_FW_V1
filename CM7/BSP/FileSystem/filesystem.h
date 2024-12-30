/*
 * filesystem.h
 *
 *  Created on: Dec 18, 2024
 *      Author: CAO HIEU
 */

#ifndef FILESYSTEM_FILESYSTEM_H_
#define FILESYSTEM_FILESYSTEM_H_
#include "main.h"
typedef enum{
	SDFS_READY = 0,
	SDFS_RELEASE,
	SDFS_BUSY,
	SDFS_ERROR
}SDFS_StateTypedef;
extern SDFS_StateTypedef SDFS_State;

void SD_Lockin(void);
void SD_Release(void);
void FS_ListFiles_path(void);
void Link_SDFS_Driver(void);
int Vim_SDFS(const char *filename, const char *content);
int Nano_SDFS(const char *filename, const char *content);
int Cat_SDFS(const char *filename);
#endif /* FILESYSTEM_FILESYSTEM_H_ */
