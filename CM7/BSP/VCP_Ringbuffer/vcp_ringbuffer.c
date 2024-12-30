/*
 * vcp_ringbuffer.c
 *
 *  Created on: Dec 16, 2024
 *      Author: CAO HIEU
 */

#include "vcp_ringbuffer.h"
#include "stdio.h"

#define CDC_BUFFER_SIZE 512

uint8_t vcp_rx_buffer_data[CDC_BUFFER_SIZE];


struct fifo vcp_rx_buffer = {
    .buffer = vcp_rx_buffer_data,
    .head = 0,
    .tail = 0,
    .size = CDC_BUFFER_SIZE
};

int fifo_is_full(struct fifo *f) {
    return ((f->head + 1) % f->size) == f->tail;
}

int fifo_is_empty(struct fifo *f) {
    return f->head == f->tail;
}

void fifo_push(struct fifo *f, uint8_t data) {
    if (!fifo_is_full(f)) {
        f->buffer[f->head] = data;
        f->head = (f->head + 1) % f->size;
    } else {
        printf("FIFO full, data lost\r\n");
    }
}

int fifo_pop(struct fifo *f) {
    if (!fifo_is_empty(f)) {
        uint8_t data = f->buffer[f->tail];
        f->tail = (f->tail + 1) % f->size;
        return data;
    } else {
        printf("FIFO empty\r\n");
        return -1;
    }
}
