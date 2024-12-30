/*
 * command.h
 *
 *  Created on: Dec 8, 2024
 *      Author: CAO HIEU
 */

#ifndef CMDLINE_COMMAND_H_
#define CMDLINE_COMMAND_H_

#include "cmdline.h"
#include <string.h>

#define	COMMAND_MAX_LENGTH	64
#define MAX_HISTORY 5
#define MAX_CMD_LENGTH COMMAND_MAX_LENGTH

void	cmdline_init();
void	command_create_task(void);


#endif /* CMDLINE_COMMAND_H_ */
