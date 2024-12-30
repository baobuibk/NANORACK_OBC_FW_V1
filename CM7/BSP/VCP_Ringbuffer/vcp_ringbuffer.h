/*
 * vcp_ringbuffer.h
 *
 *  Created on: Dec 16, 2024
 *      Author: CAO HIEU
 */

#ifndef VCP_RINGBUFFER_VCP_RINGBUFFER_H_
#define VCP_RINGBUFFER_VCP_RINGBUFFER_H_

#include "stdint.h"

typedef struct fifo {
    uint8_t *buffer;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
} fifo;

extern fifo vcp_rx_buffer;

int fifo_is_full(fifo *f);
int fifo_is_empty(fifo *f);
void fifo_push(fifo *f, uint8_t data);
int fifo_pop(fifo *f);

#endif /* VCP_RINGBUFFER_VCP_RINGBUFFER_H_ */
