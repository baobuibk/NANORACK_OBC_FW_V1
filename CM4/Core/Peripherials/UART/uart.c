/*
 * uart.c
 *
 *  Created on: May 23, 2024
 *      Author: CAO HIEU
 */

#include "uart.h"
#include <string.h>
#include "stm32h7xx_ll_gpio.h"
#include "main.h"

USART_TypeDef *uart7 = UART7;				//RS485 Interface

#define TIMEOUT_DEF 500  // 500ms timeout
uint16_t timeout;

#define ATOMIC_BLOCK_START(uart) \
    if (uart == UART7) LL_USART_DisableIT_RXNE(uart7);
//    else if (uart == USART2) LL_USART_DisableIT_RXNE(uart2);
//    else if (uart == USART3) LL_USART_DisableIT_RXNE(uart3);
//    else if (uart == UART4) LL_USART_DisableIT_RXNE(uart4);
//    else if (uart == UART5) LL_USART_DisableIT_RXNE(uart5);
//    else LL_USART_DisableIT_RXNE(uart6);

#define ATOMIC_BLOCK_END(uart) \
    if (uart == UART7) LL_USART_EnableIT_RXNE(uart7);
//    else if (uart == USART2) LL_USART_EnableIT_RXNE(uart2);
//    else if (uart == USART3) LL_USART_EnableIT_RXNE(uart3);
//    else if (uart == UART4) LL_USART_EnableIT_RXNE(uart4);
//    else if (uart == UART5) LL_USART_EnableIT_RXNE(uart5);
//    else LL_USART_EnableIT_RXNE(uart6);


/* put the following in the ISR
extern void Uart_isr (USART_TypeDef *uart);
extern uint16_t timeout;
*/


ring_buffer rx_buffer7 = { (unsigned char[UART7_BUFFER_SIZE]){0}, 0, 0, UART7_BUFFER_SIZE };
ring_buffer tx_buffer7 = { (unsigned char[UART7_BUFFER_SIZE]){0}, 0, 0, UART7_BUFFER_SIZE };
//ring_buffer rx_buffer2 = { (unsigned char[UART2_BUFFER_SIZE]){0}, 0, 0, UART2_BUFFER_SIZE };
//ring_buffer tx_buffer2 = { (unsigned char[UART2_BUFFER_SIZE]){0}, 0, 0, UART2_BUFFER_SIZE };
//ring_buffer rx_buffer3 = { (unsigned char[UART3_BUFFER_SIZE]){0}, 0, 0, UART3_BUFFER_SIZE };
//ring_buffer tx_buffer3 = { (unsigned char[UART3_BUFFER_SIZE]){0}, 0, 0, UART3_BUFFER_SIZE };
//ring_buffer rx_buffer4 = { (unsigned char[UART4_BUFFER_SIZE]){0}, 0, 0, UART4_BUFFER_SIZE };
//ring_buffer tx_buffer4 = { (unsigned char[UART4_BUFFER_SIZE]){0}, 0, 0, UART4_BUFFER_SIZE };
//ring_buffer rx_buffer5 = { (unsigned char[UART5_BUFFER_SIZE]){0}, 0, 0, UART5_BUFFER_SIZE };
//ring_buffer tx_buffer5 = { (unsigned char[UART5_BUFFER_SIZE]){0}, 0, 0, UART5_BUFFER_SIZE };
//ring_buffer rx_buffer6 = { (unsigned char[UART6_BUFFER_SIZE]){0}, 0, 0, UART6_BUFFER_SIZE };
//ring_buffer tx_buffer6 = { (unsigned char[UART6_BUFFER_SIZE]){0}, 0, 0, UART6_BUFFER_SIZE };


//ring_buffer rx_buffer1 = { { 0 }, 0, 0};
//ring_buffer tx_buffer1 = { { 0 }, 0, 0};
//ring_buffer rx_buffer2 = { { 0 }, 0, 0};
//ring_buffer tx_buffer2 = { { 0 }, 0, 0};
//ring_buffer rx_buffer3 = { { 0 }, 0, 0};
//ring_buffer tx_buffer3 = { { 0 }, 0, 0};
//ring_buffer rx_buffer5 = { { 0 }, 0, 0};
//ring_buffer tx_buffer5 = { { 0 }, 0, 0};
//ring_buffer rx_buffer6 = { { 0 }, 0, 0};
//ring_buffer tx_buffer6 = { { 0 }, 0, 0};

ring_buffer *_rx_buffer7;
ring_buffer *_tx_buffer7;
//ring_buffer *_rx_buffer2;
//ring_buffer *_tx_buffer2;
//ring_buffer *_rx_buffer3;
//ring_buffer *_tx_buffer3;
//ring_buffer *_rx_buffer4;
//ring_buffer *_tx_buffer4;
//ring_buffer *_rx_buffer5;
//ring_buffer *_tx_buffer5;
//ring_buffer *_rx_buffer6;
//ring_buffer *_tx_buffer6;

void store_char(unsigned char c, ring_buffer *buffer);

void Ringbuf_init(void)
{
    _rx_buffer7 = &rx_buffer7;
    _tx_buffer7 = &tx_buffer7;
//    _rx_buffer2 = &rx_buffer2;
//    _tx_buffer2 = &tx_buffer2;
//    _rx_buffer3 = &rx_buffer3;
//    _tx_buffer3 = &tx_buffer3;
//    _rx_buffer4 = &rx_buffer4;
//    _tx_buffer4 = &tx_buffer4;
//    _rx_buffer5 = &rx_buffer5;
//    _tx_buffer5 = &tx_buffer5;
//    _rx_buffer6 = &rx_buffer6;
//    _tx_buffer6 = &tx_buffer6;

    LL_USART_EnableIT_ERROR(uart7);
//    LL_USART_EnableIT_ERROR(uart2);
//    LL_USART_EnableIT_ERROR(uart3);
//    LL_USART_EnableIT_ERROR(uart4);
//    LL_USART_EnableIT_ERROR(uart5);
//    LL_USART_EnableIT_ERROR(uart6);

    LL_USART_EnableIT_RXNE(uart7);
//    LL_USART_EnableIT_RXNE(uart2);
//    LL_USART_EnableIT_RXNE(uart3);
//    LL_USART_EnableIT_RXNE(uart4);
//    LL_USART_EnableIT_RXNE(uart5);
//    LL_USART_EnableIT_RXNE(uart6);

}

void store_char(unsigned char c, ring_buffer *buffer)
{
  int i = (unsigned int)(buffer->head + 1) % buffer->size;

  if(i != buffer->tail) {
    ATOMIC_BLOCK_START(UART7)
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
    ATOMIC_BLOCK_END(UART7)
  }
}

int Uart_read(USART_TypeDef *uart)
{
    ring_buffer *_rx_buffer;

    switch ((uint32_t)uart) {
        case (uint32_t)UART7:
            _rx_buffer = _rx_buffer7;
        	break;
        default:
            return -1;
    }

    if (_rx_buffer->head == _rx_buffer->tail) {
        return -1;
    } else {
        ATOMIC_BLOCK_START(uart)
        unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
        _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % _rx_buffer->size;
        ATOMIC_BLOCK_END(uart)
        return c;
    }
}


void Uart_write(USART_TypeDef *uart, int c)
{
    ring_buffer *_tx_buffer;

    switch ((uint32_t)uart) {
        case (uint32_t)UART7:
            _tx_buffer = _tx_buffer7;
            break;
        default:
            return;
    }

    if (c >= 0) {
        int i = (_tx_buffer->head + 1) % _tx_buffer->size;

        ATOMIC_BLOCK_START(uart)
        while (i == _tx_buffer->tail); // Chờ cho đến khi có không gian trong bộ đệm

        _tx_buffer->buffer[_tx_buffer->head] = (uint8_t)c;
        _tx_buffer->head = i;
        ATOMIC_BLOCK_END(uart)

        // Kích hoạt ngắt TXE
        LL_USART_EnableIT_TXE(uart);
    }
}

/* checks if the new data is available in the incoming buffer
 */
int IsDataAvailable(USART_TypeDef *uart)
{
    ring_buffer *_rx_buffer;

    switch ((uint32_t)uart) {
        case (uint32_t)UART7:
            _rx_buffer = _rx_buffer7;
            break;
        default:
            return 0;
    }

    return (uint16_t)(_rx_buffer->size + _rx_buffer->head - _rx_buffer->tail) % _rx_buffer->size;
}
/* sends the string to the uart
 */
void Uart_sendstring (USART_TypeDef *uart, const char *s)
{
	while(*s) Uart_write(uart, *s++);
}

void GetDataFromBuffer (char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto)
{
	int startStringLength = strlen (startString);
	int endStringLength   = strlen (endString);
	int so_far = 0;
	int indx = 0;
	int startposition = 0;
	int endposition = 0;

repeat1:
	while (startString[so_far] != buffertocopyfrom[indx]) indx++;
	if (startString[so_far] == buffertocopyfrom[indx])
	{
		while (startString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == startStringLength) startposition = indx;
	else
	{
		so_far =0;
		goto repeat1;
	}

	so_far = 0;

repeat2:
	while (endString[so_far] != buffertocopyfrom[indx]) indx++;
	if (endString[so_far] == buffertocopyfrom[indx])
	{
		while (endString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == endStringLength) endposition = indx-endStringLength;
	else
	{
		so_far =0;
		goto repeat2;
	}

	so_far = 0;
	indx=0;

	for (int i=startposition; i<endposition; i++)
	{
		buffertocopyinto[indx] = buffertocopyfrom[i];
		indx++;
	}


}

void Uart_flush (USART_TypeDef *uart)
{
	  ring_buffer *_rx_buffer;

	  switch ((uint32_t)uart) {
	      case (uint32_t)UART7:
	          _rx_buffer = _rx_buffer7;
	          break;
	      default:
	          return ;
	  }

	  memset(_rx_buffer->buffer,'\0', _rx_buffer->size);
	  _rx_buffer->head = 0;
	  _rx_buffer->tail = 0;
}

int Uart_peek(USART_TypeDef *uart)
{
	  ring_buffer *_rx_buffer;
	  switch ((uint32_t)uart) {
	      case (uint32_t)UART7:
	          _rx_buffer = _rx_buffer7;
	          break;
	      default:
	          return 0;
	  }

	  if(_rx_buffer->head == _rx_buffer->tail)
	  {
	    return -1;
	  }
	  else
	  {
	    return _rx_buffer->buffer[_rx_buffer->tail];
	  }
}

int Copy_upto (USART_TypeDef *uart, char *string, char *buffertocopyinto)
{
	  ring_buffer *_rx_buffer;
	  switch ((uint32_t)uart) {
	      case (uint32_t)UART7:
	          _rx_buffer = _rx_buffer7;
	          break;
	      default:
	          return 0;
	  }


	int so_far =0;
	int len = strlen (string);
	int indx = 0;

again:
	while (Uart_peek(uart) != string[so_far])
		{
			buffertocopyinto[indx] = _rx_buffer->buffer[_rx_buffer->tail];
			_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % _rx_buffer->size;
			indx++;
			while (!IsDataAvailable(uart));

		}
	while (Uart_peek(uart) == string [so_far])
	{
		so_far++;
		buffertocopyinto[indx++] = Uart_read(uart);
		if (so_far == len) return 1;
		timeout = TIMEOUT_DEF;
		while ((!IsDataAvailable(uart))&&timeout);
		if (timeout == 0) return 0;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return 0;
}

int Get_after (USART_TypeDef *uart, char *string, uint8_t numberofchars, char *buffertosave)
{
	for (int indx=0; indx<numberofchars; indx++)
	{
		timeout = TIMEOUT_DEF;
		while ((!IsDataAvailable(uart))&&timeout);  // wait until some data is available
		if (timeout == 0) return 0;  // if data isn't available within time, then return 0
		buffertosave[indx] = Uart_read(uart);  // save the data into the buffer... increments the tail
	}
	return 1;
}

int Wait_for (USART_TypeDef *uart, char *string)
{
	  ring_buffer *_rx_buffer;
	  switch ((uint32_t)uart) {
	      case (uint32_t)UART7:
	          _rx_buffer = _rx_buffer7;
	          break;
	      default:
	          return 0;
	  }
	int so_far =0;
	int len = strlen (string);

again:
	timeout = TIMEOUT_DEF;
	while ((!IsDataAvailable(uart))&&timeout);  // let's wait for the data to show up
	if (timeout == 0) return 0;
	while (Uart_peek(uart) != string[so_far])  // peek in the rx_buffer to see if we get the string
	{
		if (_rx_buffer->tail != _rx_buffer->head)
		{
			_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % _rx_buffer->size;  // increment the tail
		}

		else
		{
			return 0;
		}
	}
	while (Uart_peek(uart) == string [so_far]) // if we got the first letter of the string
	{
		// now we will peek for the other letters too
		so_far++;
		_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % _rx_buffer->size;  // increment the tail
		if (so_far == len) return 1;
		timeout = TIMEOUT_DEF;
		while ((!IsDataAvailable(uart))&&timeout);
		if (timeout == 0) return 0;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return 0;
}

void Uart_isr (USART_TypeDef *uart)
{
  //  uint32_t isrflags   = LL_USART_ReadReg(uart, SR);
    ring_buffer *_rx_buffer;
    ring_buffer *_tx_buffer;

    switch ((uint32_t)uart) {
        case (uint32_t)UART7:
            _rx_buffer = _rx_buffer7;
            _tx_buffer = _tx_buffer7;
            break;
        default:
            return;
    }

    /* if DR is not empty and the Rx Int is enabled */
    if ((LL_USART_IsActiveFlag_RXNE(uart) != RESET) && (LL_USART_IsEnabledIT_RXNE(uart) != RESET))
    {
//        LL_USART_ReceiveData8(uart);                /* Read status register */
//        unsigned char c = LL_USART_ReceiveData8(uart);    /* Read data register */
//        store_char (c, _rx_buffer);  // store data in buffer
//        return;

        unsigned char data = LL_USART_ReceiveData8(uart);
        if ((LL_USART_IsActiveFlag_ORE(uart) != RESET) ||
            (LL_USART_IsActiveFlag_FE(uart) != RESET) ||
            (LL_USART_IsActiveFlag_NE(uart) != RESET))
        {
          // if error, del flag
          LL_USART_ClearFlag_ORE(uart);
          LL_USART_ClearFlag_FE(uart);
          LL_USART_ClearFlag_NE(uart);
        }
        else
        {
          store_char(data, _rx_buffer); // store data in buffer
        }
        return;


    }
    /*If interrupt is caused due to Transmit Data Register Empty */
    if ((LL_USART_IsActiveFlag_TXE(uart) != RESET) && (LL_USART_IsEnabledIT_TXE(uart) != RESET))
    {
        if(_tx_buffer->head == _tx_buffer->tail)
        {
          // Buffer empty, so disable interrupts
          LL_USART_DisableIT_TXE(uart);
//          if (uart == UART5)
//          {
//              LL_USART_EnableIT_TC(uart);
//          }
        }
        else
        {
          // There is more data in the output buffer. Send the next byte
          unsigned char c = _tx_buffer->buffer[_tx_buffer->tail];
          _tx_buffer->tail = (_tx_buffer->tail + 1) % _tx_buffer->size;

          LL_USART_TransmitData8(uart, c);
        }
        return;
    }

//    //Transmission complete Flag
//    if ((LL_USART_IsActiveFlag_TC(uart) != RESET) && (LL_USART_IsEnabledIT_TC(uart) != RESET))
//    {
//        if (uart == UART5)
//        {
//            LL_GPIO_ResetOutputPin(DERE_CTRL_GPIO_Port, DERE_CTRL_Pin);
//
//        }
//        LL_USART_DisableIT_TC(uart);
//        LL_USART_ClearFlag_TC(uart);
//        return;
//    }
}
