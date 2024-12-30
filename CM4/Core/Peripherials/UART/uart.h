/*
 * uart.h
 *
 *  Created on: May 23, 2024
 *      Author: CAO HIEU
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include "stm32h7xx_ll_usart.h"

/* change the size of the buffer */
#define UART7_BUFFER_SIZE 256
//#define UART2_BUFFER_SIZE 1024
//#define UART3_BUFFER_SIZE 256
//#define UART4_BUFFER_SIZE 512
//#define UART5_BUFFER_SIZE 512
//#define UART6_BUFFER_SIZE 512

typedef struct
{
    unsigned char *buffer;
    volatile unsigned int head;
    volatile unsigned int tail;
    unsigned int size;
} ring_buffer;

/* Initialize the ring buffer */
void Ringbuf_init(void);

/* reads the data in the rx_buffer and increment the tail count in rx_buffer */
int Uart_read(USART_TypeDef *uart);

/* writes the data to the tx_buffer and increment the head count in tx_buffer */
void Uart_write(USART_TypeDef *uart, int c);

/* function to send the string to the uart */
void Uart_sendstring(USART_TypeDef *uart, const char *s);

/* Print a number with any base
 * base can be 10, 8 etc*/
void Uart_printbase (USART_TypeDef *uart, long n, uint8_t base);

/* Checks if the data is available to read in the rx_buffer */
int IsDataAvailable(USART_TypeDef *uart);

/* Look for a particular string in the given buffer
 * @return 1, if the string is found and -1 if not found
 * @USAGE:: if (Look_for ("some string", buffer)) do something
 */
int Look_for (char *str, char *buffertolookinto);

/* Copies the required data from a buffer
 * @startString: the string after which the data need to be copied
 * @endString: the string before which the data need to be copied
 * @USAGE:: GetDataFromBuffer ("name=", "&", buffertocopyfrom, buffertocopyinto);
 */
void GetDataFromBuffer (char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto);

/* Resets the entire ring buffer, the new data will start from position 0 */
void Uart_flush (USART_TypeDef *uart);

/* Peek for the data in the Rx Bffer without incrementing the tail count
* Returns the character
* USAGE: if (Uart_peek (USART1) == 'M') do something
*/
int Uart_peek(USART_TypeDef *uart);

/* Copy the data from the Rx buffer into the buffer, Upto and including the entered string
* This copying will take place in the blocking mode, so you won't be able to perform any other operations
* Returns 1 on success and -1 otherwise
* USAGE: while (!(Copy_Upto (USART1, "some string", buffer)));
*/
int Copy_upto (USART_TypeDef *uart, char *string, char *buffertocopyinto);

/* Copies the entered number of characters (blocking mode) from the Rx buffer into the buffer, after some particular string is detected
* Returns 1 on success and -1 otherwise
* USAGE: while (!(Get_after (USART1, "some string", 6, buffer)));
*/
int Get_after (USART_TypeDef *uart, char *string, uint8_t numberofchars, char *buffertosave);

/* Wait until a particular string is detected in the Rx Buffer
* Return 1 on success and -1 otherwise
* USAGE: while (!(Wait_for(USART1, "some string")));
*/
int Wait_for (USART_TypeDef *uart, char *string);

/* the ISR for the uart. put it in the IRQ handler */
void Uart_isr (USART_TypeDef *uart);

#endif /* UART_UART_H_ */
