/*
 * command.c
 *
 *  Created on: Dec 8, 2024
 *      Author: CAO HIEU
 */

#include "command.h"
#include <stdlib.h>

#include <stdio.h>
#include "vt100.h"

#include "scheduler.h"
#include "usbd_cdc_if.h"
#include "rtc_rv3149c3.h"

#include "filesystem.h"
#include "ring_buffer.h"
#include "../Task/Fake_EXP/fake_exp.h"


#define NAME_SHELL "\033[1;32mOBC_Dual\033[1;37m"

typedef struct _Command_TaskContextTypedef_
{
	SCH_TASK_HANDLE               taskHandle;
	SCH_TaskPropertyTypedef       taskProperty;
} Command_TaskContextTypedef;

typedef struct {
    char commandBuffer[COMMAND_MAX_LENGTH];
    uint16_t commandBufferIndex;
    char commandHistory[MAX_HISTORY][COMMAND_MAX_LENGTH];
    uint16_t historyCount;
    uint16_t historyIndex;
} CMDLine_Context;

typedef enum {
    CMDLINE_MODE_CLI,
    CMDLINE_MODE_GUI,
	CMDLINE_MODE_MONITOR
} CMDLine_Mode;



CMDLine_Mode currentMode = CMDLINE_MODE_CLI;

CMDLine_Context pContext = {0};

/*******************************************************************************
 *
 * 	Internal functions declaration
 *
 ******************************************************************************/
void process_gui_command(char rxData);
void process_command(char rxData, CMDLine_Context* context);
void gui_cover_generator();
void menu_generator();
void cursor_move_to(int x, int y);
void monitor_cover_generator(void);
static	void cmdline_task_update(void);
int CMD_help(int argc, char *argv[]);
int CMD_welcome(int argc, char *argv[]);
int CMD_cls(int argc, char *argv[]);
int CMD_gui_ast(int argc, char *argv[]);
int NotYetDefine(int argc, char *argv[]);
int CMD_sd_release(int argc, char *argv[]);
int CMD_sd_lockin(int argc, char *argv[]);
int CMD_sd_out(int argc, char *argv[]);
int CMD_time(int argc, char *argv[]);
int CMD_ls(int argc, char *argv[]);
int CMD_vim(int argc, char *argv[]);
int CMD_nano(int argc, char *argv[]);
int CMD_cat(int argc, char *argv[]);
int CMD_reset(int argc, char *argv[]);
int CMD_cm4(int argc, char *argv[]);
int CMD_time_set(int argc, char *argv[]);
int CMD_lz_ctrl(int argc, char *argv[]);
int CMD_lz_cur(int argc, char *argv[]);
int CMD_lz_vol(int argc, char *argv[]);
int CMD_exp_tec(int argc, char *argv[]);
int CMD_pd_poll(int argc, char *argv[]);
int CMD_sd_reset(int argc, char *argv[]);
int CMD_monitor(int argc, char *argv[]);
const char * ErrorCode[6] = {"\033[1;32mOK\033[1;37m\r\n", "\033[1;31mBAD_CMD\033[1;37m - Try \033[1;35m[help]\033[1;37m\r\n", "\033[1;33mTOO_MANY_ARGS\033[1;37m\r\n",
"\033[1;33mTOO_FEW_ARGS\033[1;37m\r\n", "\033[1;31mINVALID_ARG\033[1;37m\r\n", "\033[1;35mPENDING...\033[1;37m\r\n" };

tCmdLineEntry g_psCmdTable[] = {
    {"help", CMD_help, "Show available commands | format: help"},
    {"welcome", CMD_welcome, "Display welcome screen | format: welcome"},
    {"cls", CMD_cls, "Clear terminal screen | format: cls"},
    {"reset", CMD_reset, "Reset MCU | format: reset"},

    {"time_set", CMD_time_set, "Set RTC time | format: time_set YYYY MM DD hh mm ss"},
    {"time", CMD_time, "Display RTC time | format: time <hard|soft|sync>"},

    {"gui_ast", CMD_gui_ast, "Enter GUI Assistant mode | format: gui_ast\r\n\r\n ------> \033[7m[SD Control]\033[27m"},

    {"sd_release", CMD_sd_release, "Release SD card | format: sd_release"},
    {"sd_lockin", CMD_sd_lockin, "Lock SD card | format: sd_lockin"},
    {"sd_reset", CMD_sd_reset, "Reset SD interface | format: sd_reset"},
    {"ls", CMD_ls, "List files in SD card | format: ls\r\n\r\n ------> \033[7m[File system]\033[27m"},
    {"vim", CMD_vim, "Write new line to file | format: vim <filename> <content>"},
    {"nano", CMD_nano, "Overwrite file content | format: nano <filename> <content>"},
    {"cat", CMD_cat, "Display file content | format: cat <filename>"},

    {"cm4", CMD_cm4, "Send JSON command to CM4 | format: cm4 <json_command>\r\n\r\n ------> \033[7m[Low Layer Control]\033[27m"},

    {"exp_tec", CMD_exp_tec, "Control TEC module | format: exp_tec <ena|dis|set <temp>|get>"},
    {"lz_ctrl", CMD_lz_ctrl, "Control laser slot | format: lz_ctrl <slot_num> (0=off, 1-48:on)"},
    {"lz_vol", CMD_lz_vol, "Set laser DAC voltage | format: lz_vol <dac_value> (0-255)"},
    {"lz_cur", CMD_lz_cur, "Get laser current | format: lz_cur"},

    {"pd_poll", CMD_pd_poll, "Poll photodiode data | format: pd_poll <pd_num> (1-48)"},
    {"monitor", CMD_monitor, "Enter monitor mode | format: monitor"},
    {0, 0, 0}
};



//volatile static	ring_buffer *p_CommandRingBuffer;
static	char s_commandBuffer[COMMAND_MAX_LENGTH];
static uint8_t	s_commandBufferIndex = 0;

static Command_TaskContextTypedef           s_CommandTaskContext =
{
	SCH_INVALID_TASK_HANDLE,                 // Will be updated by Schedular
	{
		SCH_TASK_SYNC,                      // taskType;
		SCH_TASK_PRIO_0,                    // taskPriority;
		10,                                // taskPeriodInMS;
		cmdline_task_update,                // taskFunction;
		9
	}
};

void welcome(){

    TERMINAL_BACK_DEFAULT(); /* set terminal background color: black */
    TERMINAL_DISPLAY_CLEAR();
    TERMINAL_RESET_CURSOR();
    TERMINAL_FONT_CYAN();
    CDC_SendString("┌─────────────────────────────────────────────────────────────┐\r\n");
    CDC_SendString("│       █▀ █▀█ ▄▀█ █▀▀ █▀▀ █   █ █ █▄ █ ▀█▀ █▀▀ █▀▀ █ █       │\r\n");
    CDC_SendString("│       ▄█ █▀▀ █▀█ █▄▄ ██▄ █▄▄ █ █ █ ▀█  █  ██▄ █▄▄ █▀█       │\r\n");
    CDC_SendString("└─────────────────────────────────────────────────────────────┘\r\n");
 	CDC_SendString("░█████╗░██████╗░░█████╗░░░░░░░██████╗░██╗░░░██╗░█████╗░██╗░░░░░\r\n");
 	CDC_SendString("██╔══██╗██╔══██╗██╔══██╗░░░░░░██╔══██╗██║░░░██║██╔══██╗██║░░░░░\r\n");
 	CDC_SendString("██║░░██║██████╦╝██║░░╚═╝█████╗██║░░██║██║░░░██║███████║██║░░░░░\r\n");
    CDC_SendString("██║░░██║██╔══██╗██║░░██╗╚════╝██║░░██║██║░░░██║██╔══██║██║░░░░░\r\n");
 	CDC_SendString("╚█████╔╝██████╦╝╚█████╔╝░░░░░░██████╔╝╚██████╔╝██║░░██║███████╗\r\n");
 	CDC_SendString("░╚════╝░╚═════╝░░╚════╝░░░░░░░╚═════╝░░╚═════╝░╚═╝░░╚═╝╚══════╝\r\n");
 	CDC_SendString("├─────────────────────────────────────────────────────────────┤\r\n");
    TERMINAL_FONT_DEFAULT();
    HAL_Delay(50);
 	TERMINAL_FONT_RED();
 	CDC_SendString("--->>> Use key \"-\" or  key \"=\" for history and \\ for Suggestions\r\n");
 	CDC_SendString("> ");
    TERMINAL_FONT_DEFAULT();
//    PRINT_CLI_NAME();
    TERMINAL_SHOW_CURSOR();


}

void	cmdline_init()
{
	currentMode = CMDLINE_MODE_CLI;
//	p_CommandRingBuffer = uart_get_uart0_rx_buffer_address();
	memset((void *)s_commandBuffer, 0, sizeof(s_commandBuffer));
	s_commandBufferIndex = 0;

    welcome();
//    UART_Flush_RingTx(UART_CMDLINE);

}

static void cmdline_task_update(void) {
    char rxData;

    if (CDC_IsDataAvailable()) {

        rxData = CDC_ReadChar();

        if(rxData == 27){
        	monitor = 0;
        	cmdline_init();
        	currentMode = CMDLINE_MODE_CLI;
        }else{
          char str[2] = {rxData};
      	  CDC_SendString(str);
        }

        if (currentMode == CMDLINE_MODE_CLI) {
            process_command(rxData, &pContext);
        } else if (currentMode == CMDLINE_MODE_GUI) {
            process_gui_command(rxData);
        }
    }
}

int CMD_gui_ast(int argc, char *argv[]) {
    if (argc != 1) {
        return CMDLINE_TOO_MANY_ARGS;
    }

    currentMode = CMDLINE_MODE_GUI;
	welcome();

	// Return success.
    TERMINAL_BACK_DEFAULT(); /* set terminal background color: black */
    TERMINAL_FONT_DEFAULT(); /* set terminal display color: green */

    /* This prints the clear screen and move cursor to top-left corner control
     * characters for VT100 terminals. This means it will not work on
     * non-VT100 compliant terminals, namely Windows' cmd.exe, but should
     * work on anything unix-y. */
    TERMINAL_RESET_CURSOR();
    TERMINAL_DISPLAY_CLEAR();

    CDC_SendString("Entering GUI mode...\r\n");
    CDC_SendString("OBC Board Dual Rev1.0\r\n\r\n");
    gui_cover_generator();
    menu_generator();



    return CMDLINE_NONE_RETURN;
}

int CMD_monitor(int argc, char *argv[]) {
    if (argc != 1) {
        return CMDLINE_TOO_MANY_ARGS;
    }
//
//    currentMode = CMDLINE_MODE_MONITOR;

	welcome();

	// Return success.
    TERMINAL_BACK_DEFAULT(); /* set terminal background color: black */
    TERMINAL_FONT_DEFAULT(); /* set terminal display color: green */

    /* This prints the clear screen and move cursor to top-left corner control
     * characters for VT100 terminals. This means it will not work on
     * non-VT100 compliant terminals, namely Windows' cmd.exe, but should
     * work on anything unix-y. */
    TERMINAL_RESET_CURSOR();
    TERMINAL_DISPLAY_CLEAR();

    CDC_SendString("Entering Monitor Mode...\r\n");
    CDC_SendString("OBC Board Dual Rev1.0\r\n\r\n");
    monitor_cover_generator();

	cursor_move_to(8, 7);
	CDC_SendString("EXP Tec Mode:");

	cursor_move_to(8, 8);
	CDC_SendString("Set Temperature:");

	cursor_move_to(8, 9);
	CDC_SendString("Current Temperature:");

	cursor_move_to(8, 11);

	const char *bottomLine = "──────────────────────────────────────────────────────────────────────\r\n";
    CDC_SendString(bottomLine);

	cursor_move_to(8, 13);
	CDC_SendString("Laser Slot:");

	cursor_move_to(8, 14);
	CDC_SendString("Laser Current:");

	cursor_move_to(8, 15);
	CDC_SendString("Laser Voltage:");

	cursor_move_to(8, 16);
	CDC_SendString("Photo Voltage:");

	cursor_move_to(32, 21);
	CDC_SendString("<Key ESC to Exit>");


    monitor = 1;

    return CMDLINE_NONE_RETURN;

}

void menu_generator(){
	cursor_move_to(8, 7);
	TERMINAL_HIGHLIGHT();
	CDC_SendString("1  ");
	CDC_SendString("System Options");

	CDC_SendString("            Do Something with Options in here    ");
	TERMINAL_UN_HIGHLIGHT();
	cursor_move_to(8, 8);
	CDC_SendString("2  ");
	CDC_SendString("Display Options");

	CDC_SendString("           Do Something with Options in here    ");
	cursor_move_to(8, 9);
	CDC_SendString("3  ");
	CDC_SendString("LED Options");

	CDC_SendString("               Do Something with Options in here    ");

	cursor_move_to(32, 21);
	CDC_SendString("<Key ESC to Exit>");
}

void cursor_move_to(int x, int y){
	// ->x
	//   y
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "\033[%d;%dH", (y), (x));
    CDC_SendString(buffer);
}


void monitor_cover_generator(){
    const char *topLine =    "┌──────────────────────────┤ OBCDual GUI >Monitor< ├───────────────────────────┐\r\n";
    const char *middleLine = "│                                                                              │\r\n";
    const char *bottomLine = "└──────────────────────────────────────────────────────────────────────────────┘\r\n";
    CDC_SendString(topLine);
    for(int i = 0; i < 17 ; i++){
    	CDC_SendString(middleLine);
    }
    CDC_SendString(bottomLine);

}

void gui_cover_generator(){
    const char *topLine =    "┌──────────────────────────┤ OBCDual GUI ASSISTANT ├───────────────────────────┐\r\n";
    const char *middleLine = "│                                                                              │\r\n";
    const char *bottomLine = "└──────────────────────────────────────────────────────────────────────────────┘\r\n";
    CDC_SendString(topLine);
    for(int i = 0; i < 17 ; i++){
    	CDC_SendString(middleLine);
    }
    CDC_SendString(bottomLine);

}


void process_gui_command(char rxData) {

}


static int find_matches(const char *input, char matches[][COMMAND_MAX_LENGTH], int max_matches) {
    int count = 0;

    for (int i = 0; g_psCmdTable[i].pcCmd != NULL; i++) {
        if (strncmp(g_psCmdTable[i].pcCmd, input, strlen(input)) == 0) {
            strncpy(matches[count], g_psCmdTable[i].pcCmd, COMMAND_MAX_LENGTH);
            matches[count][COMMAND_MAX_LENGTH - 1] = '\0';
            count++;
            if (count >= max_matches) {
                break;
            }
        }
    }

    return count;
}


/*-----------------------COMMAND FUNCTION LIST---------------------------*/

int CMD_help(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;

    CDC_SendString("\r\nAvailable commands:\r\n");

    tCmdLineEntry *pEntry = &g_psCmdTable[0];
    size_t maxCmdLength = 0;
    while (pEntry->pcCmd) {
        size_t cmdLength = strlen(pEntry->pcCmd);
        if (cmdLength > maxCmdLength) {
            maxCmdLength = cmdLength;
        }
        pEntry++;
    }
    pEntry = &g_psCmdTable[0];
    while (pEntry->pcCmd) {
        char buffer[256];
        size_t cmdLength = strlen(pEntry->pcCmd);
        int padding = (int)(maxCmdLength - cmdLength + 4);
        snprintf(buffer, sizeof(buffer), "\r\n[%s]%*s: %s",
                 pEntry->pcCmd, padding, "", pEntry->pcHelp);

        CDC_SendString(buffer);
        pEntry++;
    }

    return (CMDLINE_OK);
}


int CMD_welcome(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;
	welcome();
	// Return success.

	return (CMDLINE_OK);
}

int CMD_cls(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;
	welcome();
	// Return success.
    TERMINAL_BACK_DEFAULT(); /* set terminal background color: black */
    TERMINAL_FONT_DEFAULT(); /* set terminal display color: green */

    /* This prints the clear screen and move cursor to top-left corner control
     * characters for VT100 terminals. This means it will not work on
     * non-VT100 compliant terminals, namely Windows' cmd.exe, but should
     * work on anything unix-y. */
    TERMINAL_RESET_CURSOR();
    TERMINAL_DISPLAY_CLEAR();

	return (CMDLINE_OK);
}

int CMD_sd_release(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;

    SD_Release();

	return (CMDLINE_OK);
}


int CMD_sd_lockin(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;

    SD_Lockin();

    Link_SDFS_Driver();

	return (CMDLINE_OK);
}


int CMD_sd_reset(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;

    LL_GPIO_ResetOutputPin(HUB_RESET_GPIO_Port, HUB_RESET_Pin);
    HAL_Delay(200);
    LL_GPIO_SetOutputPin(HUB_RESET_GPIO_Port, HUB_RESET_Pin);
//    LL_GPIO_TogglePin(HUB_RESET_GPIO_Port, HUB_RESET_Pin);

	return (CMDLINE_OK);
}

int CMD_time(int argc, char *argv[]) {
    if (argc > 3) return CMDLINE_TOO_MANY_ARGS;
    if (argc < 2) return CMDLINE_TOO_FEW_ARGS;


    const char *operation = argv[1];

    if (strcmp(operation, "sync") == 0) {
        if (rtc_sync_time()) {
        	CDC_SendString("RTC synchronized successfully.\r\n");
        } else {
        	CDC_SendString("RTC synchronization failed.\r\n");
        }
    }
    else if (strcmp(operation, "hard") == 0) {
        rtc_date_time_t hardTime;
        if (rtc_get_hard_time(&hardTime)) {
            rtc_date_time_t softTime = {0};

            rtc_get_soft_time(&softTime);

            char buffer[128];
            snprintf(buffer, sizeof(buffer),
                     "\r\nHard-time: %04u/%02u/%02u %02u:%02u:%02u\r\n"
                     "Soft-time: %04u/%02u/%02u %02u:%02u:%02u\r\n",
                     hardTime.year, hardTime.month, hardTime.day,
                     hardTime.hour, hardTime.minute, hardTime.second,
                     softTime.year, softTime.month, softTime.day,
                     softTime.hour, softTime.minute, softTime.second);
            CDC_SendString(buffer);
        } else {
        	CDC_SendString("\r\nFailed to read hard-time from RTC.\r\n");
        }
    }
    else if (strcmp(operation, "soft") == 0) {
        rtc_date_time_t softTime = {0};
        rtc_get_soft_time(&softTime);

        char buffer[128];
        snprintf(buffer, sizeof(buffer),
                 "\r\nSoft-time: %04u/%02u/%02u %02u:%02u:%02u\r\n",
                 softTime.year, softTime.month, softTime.day,
                 softTime.hour, softTime.minute, softTime.second);
        CDC_SendString(buffer);
    }
    else {
    	CDC_SendString("\r\nInvalid command. Use 'hard', 'soft', or 'sync'.\r\n");
    }

    return CMDLINE_OK;
}



int CMD_ls(int argc, char *argv[]) {
    if ((argc) < 1) return CMDLINE_TOO_FEW_ARGS;
    if ((argc) > 1) return CMDLINE_TOO_MANY_ARGS;

    FS_ListFiles_path();

	return (CMDLINE_OK);
}
int CMD_cat(int argc, char *argv[]) {

    if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
    if (argc > 2) return CMDLINE_TOO_MANY_ARGS;

    const char *filename = argv[1];
    (void)Cat_SDFS(filename);


    return (CMDLINE_OK);
}

int CMD_nano(int argc, char *argv[]) {
    if (argc < 3) return CMDLINE_TOO_FEW_ARGS;
    if (argc > 3) return CMDLINE_TOO_MANY_ARGS;

    const char *filename = argv[1];
    const char *content = argv[2];

    (void)Nano_SDFS(filename, content);

    return (CMDLINE_OK);
}

int CMD_vim(int argc, char *argv[]) {
    if (argc < 3) return CMDLINE_TOO_FEW_ARGS;
    if (argc > 3) return CMDLINE_TOO_MANY_ARGS;

    const char *filename = argv[1];
    const char *content = argv[2];

    // Gọi hàm ghi nội dung vào hàng mới
    if (Vim_SDFS(filename, content) == 0) {
        CDC_SendString("\r\nContent written successfully.\r\n");
    } else {
        CDC_SendString("\r\nFailed to write content.\r\n");
    }

    return CMDLINE_OK;
}


int CMD_reset(int argc, char *argv[]) {
	NVIC_SystemReset();

    return (CMDLINE_OK);
}

int CMD_cm4(int argc, char *argv[]) {
    if (argc < 2) return CMDLINE_TOO_FEW_ARGS;
    if (argc > 2) return CMDLINE_TOO_MANY_ARGS;

    const char *content = argv[1];
    ringbuff_write(rb_cm7_to_cm4, content , strlen(content));
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "\r\nSend to CM4: %s , %d\r\n",
             content, strlen(content));
    CDC_SendString(buffer);
    return (CMDLINE_OK);
}

int CMD_lz_ctrl(int argc, char *argv[]) {
    if (argc != 2) {
        return CMDLINE_TOO_FEW_ARGS; // cần đúng 2 tham số: lệnh và số
    }

    int laser_num = atoi(argv[1]); // Chuyển đổi tham số thứ 2 thành số nguyên
    if (laser_num < 0 || laser_num > 48) {
        CDC_SendString("Error: Laser number must be between 0 and 48.\r\n");
        return CMDLINE_INVALID_ARG; // Kiểm tra giá trị trong khoảng hợp lệ
    }

    // Tạo chuỗi JSON theo format yêu cầu
    char content[64];
    snprintf(content, sizeof(content), "{\"i2c\":{\"laser\":\"%d\"}}", laser_num);

    laser_status.laser_slot = laser_num;

    // Gửi nội dung qua ring buffer
    ringbuff_write(rb_cm7_to_cm4, content, strlen(content));

    // Phản hồi lại qua CDC
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "\r\nLaser command sent: %s , Length: %d\r\n",
             content, strlen(content));
    CDC_SendString(buffer);

    return CMDLINE_OK;
}


int CMD_pd_poll(int argc, char *argv[]) {
    if (argc != 2) {
        return CMDLINE_TOO_FEW_ARGS; // cần đúng 2 tham số: lệnh và số
    }

    int laser_num = atoi(argv[1]); // Chuyển đổi tham số thứ 2 thành số nguyên
    if (laser_num < 1 || laser_num > 48) {
        CDC_SendString("\r\nError: Photodiode number must be between 1 and 48.\r\n");
        return CMDLINE_INVALID_ARG; // Kiểm tra giá trị trong khoảng hợp lệ
    }

    // Tạo chuỗi JSON theo format yêu cầu
    char content[64];
    snprintf(content, sizeof(content), "{\"spi\":{\"read\":\"%d\"}}", laser_num);

    // Gửi nội dung qua ring buffer
    ringbuff_write(rb_cm7_to_cm4, content, strlen(content));

    // Phản hồi lại qua CDC
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "\r\nPhotodiode command sent: %s , Length: %d\r\n",
             content, strlen(content));
    CDC_SendString(buffer);

    return CMDLINE_OK;
}

int CMD_lz_cur(int argc, char *argv[]) {
    if (argc != 1) {
        return CMDLINE_TOO_MANY_ARGS; // Không cần tham số
    }

    // Chuỗi JSON cố định
    const char *content = "{\"i2c\":{\"ampe\":\"0\"}}";

    // Gửi nội dung qua ring buffer
    ringbuff_write(rb_cm7_to_cm4, content, strlen(content));

    // Phản hồi lại qua CDC
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "\r\nCurrent laser command sent: %s , Length: %d\r\n",
             content, strlen(content));
    CDC_SendString(buffer);

    return CMDLINE_OK;
}

int CMD_lz_vol(int argc, char *argv[]) {
    if (argc != 2) {
        return CMDLINE_TOO_FEW_ARGS; // Lệnh cần đúng 2 tham số
    }

    int dac_value = atoi(argv[1]); // Chuyển đổi tham số sang số nguyên
    if (dac_value < 0 || dac_value > 255) {
        CDC_SendString("\r\nError: DAC value must be between 0 and 255.\r\n");
        return CMDLINE_INVALID_ARG; // Kiểm tra giá trị trong khoảng hợp lệ
    }

    // Tạo chuỗi JSON theo format yêu cầu
    char content[64];
    snprintf(content, sizeof(content), "{\"i2c\":{\"dac\":\"%d\"}}", dac_value);

    // Gửi nội dung qua ring buffer
    ringbuff_write(rb_cm7_to_cm4, content, strlen(content));

    // Phản hồi lại qua CDC
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "\r\nDAC command sent: %s , Length: %d\r\n",
             content, strlen(content));
    CDC_SendString(buffer);

	 laser_status.laser_voltage =(float)dac_value / 85.0;


    return CMDLINE_OK;
}





int CMD_time_set(int argc, char *argv[]) {
    // Kiểm tra số lượng tham số
    if (argc != 7) return CMDLINE_TOO_FEW_ARGS;

    // Chuyển đổi các tham số chuỗi thành số nguyên
    rtc_date_time_t dateTime;
    dateTime.year   = atoi(argv[1]); // Năm
    dateTime.month  = atoi(argv[2]); // Tháng
    dateTime.day    = atoi(argv[3]); // Ngày
    dateTime.hour   = atoi(argv[4]); // Giờ
    dateTime.minute = atoi(argv[5]); // Phút
    dateTime.second = atoi(argv[6]); // Giây

    // Kiểm tra hợp lệ của dữ liệu nhập vào
    if (dateTime.year < 2000 || dateTime.year > 2099 ||
        dateTime.month < 1 || dateTime.month > 12 ||
        dateTime.day < 1 || dateTime.day > 31 ||
        dateTime.hour < 0 || dateTime.hour > 23 ||
        dateTime.minute < 0 || dateTime.minute > 59 ||
        dateTime.second < 0 || dateTime.second > 59) {
        return (CMDLINE_INVALID_ARG);
    }

    // Gọi hàm thiết lập thời gian
    if (rtc_set_date_time(&dateTime)) {
        CDC_SendString("\r\nTime set successfully.\r\n");
    } else {
        CDC_SendString("\r\nFailed to set time.\r\n");
    }

    return CMDLINE_OK;
}


int NotYetDefine(int argc, char *argv[]) {
	CDC_SendString("\r\nThis function is not defined yet \r\n");
	// Return success.
	return (CMDLINE_OK);
}


int CMD_exp_tec(int argc, char *argv[]) {
    if (argc < 2) return CMDLINE_TOO_FEW_ARGS; // Cần ít nhất 2 tham số (lệnh và hành động)

    const char *action = argv[1];

    if (strcmp(action, "ena") == 0) {
        tec_status.enabled = true;
        CDC_SendString("\r\nTEC enabled.\r\n");
        counter_fake = 0;
    }
    else if (strcmp(action, "dis") == 0) {
        tec_status.enabled = false;
        CDC_SendString("\r\nTEC disabled.\r\n");
    }
    else if (strcmp(action, "set") == 0) {
        if (argc != 3) return CMDLINE_TOO_FEW_ARGS; // Cần thêm tham số nhiệt độ

        float temp = atof(argv[2]);
        if (temp < 0.0 || temp > 100.0) { // Giới hạn nhiệt độ 0-100 °C (có thể chỉnh sửa)
            CDC_SendString("\r\nError: Temperature must be between 0.0 and 100.0°C.\r\n");
            return CMDLINE_INVALID_ARG;
        }

        tec_status.temperature = temp;
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "\r\nTEC temperature set to %.1f°C.\r\n", temp);
        CDC_SendString(buffer);
    }
    else if (strcmp(action, "get") == 0) {
        // Giả sử giá trị hiện tại được cập nhật bởi một hàm khác
        char buffer[128];
        snprintf(buffer, sizeof(buffer),
                 "\r\nTEC Status:\r\n"
                 "  Enabled: %s\r\n"
                 "  Set Temp: %.1f°C\r\n"
                 "  Current Temp: %.1f°C\r\n",
                 tec_status.enabled ? "Yes" : "No",
                 tec_status.temperature,
                 tec_status.currentTemp);
        CDC_SendString(buffer);
    }
    else {
        CDC_SendString("\r\nInvalid action. Use 'ena', 'dis', 'set <value>', or 'get'.\r\n");
        return CMDLINE_INVALID_ARG;
    }

    return CMDLINE_OK;
}




void process_command(char rxData, CMDLine_Context* context)
{

//    static uint8_t escapeSequenceIndex = 0;  // Track escape sequence (for arrow keys)
//    static char escapeSequence[5] = {0};    // Buffer to store escape sequence
//

//    if (escapeSequenceIndex > 0 || rxData == '\x1B') {
//        escapeSequence[escapeSequenceIndex++] = rxData;
//
//        if (escapeSequenceIndex == 4 && memcmp(escapeSequence, "\x1B[3~", 4) == 0) {
//            escapeSequenceIndex = 0;
//            memset(escapeSequence, 0, sizeof(escapeSequence));
//            return;
//        }
//
//        if (escapeSequenceIndex == 3 &&
//            (memcmp(escapeSequence, KEY_UP, 3) == 0 || memcmp(escapeSequence, KEY_DOWN, 3) == 0)) {
//
////            uint8_t upordown = 0; // up
////            if (memcmp(escapeSequence, KEY_UP, 3) == 0 && context->historyIndex > 0) {
////            	return;
////            } else if (memcmp(escapeSequence, KEY_DOWN, 3) == 0 && context->historyIndex < context->historyCount) {
////            	return;
////            }
//            // Load history command
//            escapeSequenceIndex = 0; // Reset escape sequence
//            memset(escapeSequence, 0, sizeof(escapeSequence));
//            return;
//        }
//    }

    if (rxData == 27){
        char buffer[30];
        snprintf(buffer, sizeof(buffer), "\r\n%s$ ", NAME_SHELL);
        CDC_SendString(buffer);
        context->commandBufferIndex = 0;
        context->commandBuffer[0] = '\0';
    }

    if (rxData == '\\') { // '\'
        char matches[10][COMMAND_MAX_LENGTH];
        CDC_SendString("\x7f");
        int match_count = find_matches(context->commandBuffer, matches, 10);

        // Display suggestions
        CDC_SendString("\r\n\033[1;36mSuggestions:\033[1;37m\r\n");
        if (match_count > 0) {
            for (int i = 0; i < match_count; i++) {
            	CDC_SendString("[");
                CDC_SendString(matches[i]);
                CDC_SendString("]\r\n");
            }
        } else {
            CDC_SendString("No matches found.\r\n");
        }

        // Redisplay the current buffer
        char buffer[30];
        snprintf(buffer, sizeof(buffer), "\r%s$ ", NAME_SHELL);
        CDC_SendString(buffer);
        CDC_SendString(context->commandBuffer);
        return;
    }

    // Check if we are handling an escape sequence
    if (rxData == 0x2D) { // '-' key (history up)
        if (context->historyIndex > 0) {
            context->historyIndex--;
        }

        // Load history command
        if (context->historyIndex < context->historyCount) {
            strcpy(context->commandBuffer, context->commandHistory[context->historyIndex]);
            context->commandBufferIndex = strlen(context->commandBuffer);
        } else {
            context->commandBuffer[0] = '\0';
            context->commandBufferIndex = 0;
        }

        // Clear current line and display updated command
        CDC_SendString("\033[2K"); // Clear entire line
        char buffer[30];
        snprintf(buffer, sizeof(buffer), "\r%s$ ", NAME_SHELL);
        CDC_SendString(buffer);
        CDC_SendString(context->commandBuffer); // Display updated command
        return;
    } else if (rxData == 0x3D) { // '=' key (history down)
        if (context->historyIndex < context->historyCount) {
            context->historyIndex++;
        }

        // Load history command
        if (context->historyIndex < context->historyCount) {
            strcpy(context->commandBuffer, context->commandHistory[context->historyIndex]);
            context->commandBufferIndex = strlen(context->commandBuffer);
        } else {
            context->commandBuffer[0] = '\0';
            context->commandBufferIndex = 0;
        }

        // Clear current line and display updated command
        CDC_SendString("\033[2K"); // Clear entire line
        char buffer[30];
        snprintf(buffer, sizeof(buffer), "\r%s$ ", NAME_SHELL);
        CDC_SendString(buffer);
        CDC_SendString(context->commandBuffer); // Display updated command
        return;
    }

    // Handle individual key presses
    if (((rxData >= 32 && rxData <= 126) || rxData == KEY_ENTER || rxData == KEY_BACKSPACE)
    										&& rxData != 0x2D && rxData != 0x3D && rxData != 0x5C) {
        if (rxData == KEY_ENTER) {

            rtc_date_time_t softTime = {0};
            rtc_get_soft_time(&softTime);
            char xtime[30];
            snprintf(xtime, sizeof(xtime),
                     "\033[1;35m[%02u:%02u:%02u]\033[1;37m ",
                     softTime.hour, softTime.minute, softTime.second);

            if (context->commandBufferIndex > 0) {
                context->commandBuffer[context->commandBufferIndex] = '\0';

                // Save to history
                if (context->historyCount == 0 || strcmp(context->commandHistory[context->historyCount - 1], context->commandBuffer) != 0) {
                    if (context->historyCount < MAX_HISTORY) {
                        strcpy(context->commandHistory[context->historyCount], context->commandBuffer);
                        context->historyCount++;
                    } else {
                        for (int i = 0; i < MAX_HISTORY - 1; i++) {
                            strcpy(context->commandHistory[i], context->commandHistory[i + 1]);
                        }
                        strcpy(context->commandHistory[MAX_HISTORY - 1], context->commandBuffer);
                    }
                }
                context->historyIndex = context->historyCount;

                // Process command
                int8_t ret_val = CmdLineProcess(context->commandBuffer);
                if(ret_val == CMDLINE_NONE_RETURN){

                }else{
					char buffer[60];
					snprintf(buffer, sizeof(buffer), "\r\n--> Return: ");
					CDC_SendString(buffer);
					CDC_SendString(ErrorCode[ret_val]);  //
					snprintf(buffer, sizeof(buffer), "%s%s$ ",xtime , NAME_SHELL);
					CDC_SendString(buffer);


					context->commandBufferIndex = 0;
                }
            } else {
                char buffer[60];
                snprintf(buffer, sizeof(buffer), "\r\n%s%s$ ", xtime, NAME_SHELL);

                CDC_SendString(buffer);
            }
        } else if (rxData == KEY_BACKSPACE) {
            if (context->commandBufferIndex > 0) {
                context->commandBufferIndex--;
                context->commandBuffer[context->commandBufferIndex] = '\0';
            }else{
            	CDC_SendString(" ");
            }
        } else {
            if (context->commandBufferIndex < COMMAND_MAX_LENGTH - 1) {
                context->commandBuffer[context->commandBufferIndex++] = rxData;
                context->commandBuffer[context->commandBufferIndex] = '\0';
            } else {
                // Command too long
                CDC_SendString("\r\nError: Command too long.");
                char buffer[30];
                snprintf(buffer, sizeof(buffer), "\r\n%s$ ", NAME_SHELL);
                CDC_SendString(buffer);
                context->commandBufferIndex = 0;
                context->commandBuffer[0] = '\0';
            }
        }
    } else {
//    	char debugBuffer[10];
//    	sprintf(debugBuffer, "0x%02X", rxData);
//    	CDC_SendString(debugBuffer);
        // Invalid character, do nothing
    }

}




void	command_create_task(void)
{
	SCH_TASK_CreateTask(&s_CommandTaskContext.taskHandle, &s_CommandTaskContext.taskProperty);
}

