#ifndef SYSTEM_IPC_RING_BUFFER_H_
#define SYSTEM_IPC_RING_BUFFER_H_

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of ring buffer library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.3.1
 */

#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define MEM_ALIGN(x)                        (((x) + 0x00000003) & ~0x00000003)

/* Shared RAM between 2 cores is SRAM4 in D3 domain */
#define SHD_RAM_START_ADDR                  0x38000000
#define SHD_RAM_LEN                         0x0000FFFF

/* Buffer from CM4 to CM7 */
#define BUFF_CM4_TO_CM7_ADDR                MEM_ALIGN(SHD_RAM_START_ADDR)
#define BUFF_CM4_TO_CM7_LEN                 MEM_ALIGN(sizeof(ringbuff_t))
#define BUFFDATA_CM4_TO_CM7_ADDR            MEM_ALIGN(BUFF_CM4_TO_CM7_ADDR + BUFF_CM4_TO_CM7_LEN)
#define BUFFDATA_CM4_TO_CM7_LEN             MEM_ALIGN(0x00000400)

/* Buffer from CM7 to CM4 */
#define BUFF_CM7_TO_CM4_ADDR                MEM_ALIGN(BUFFDATA_CM4_TO_CM7_ADDR + BUFFDATA_CM4_TO_CM7_LEN)
#define BUFF_CM7_TO_CM4_LEN                 MEM_ALIGN(sizeof(ringbuff_t))
#define BUFFDATA_CM7_TO_CM4_ADDR            MEM_ALIGN(BUFF_CM7_TO_CM4_ADDR + BUFF_CM7_TO_CM4_LEN)
#define BUFFDATA_CM7_TO_CM4_LEN             MEM_ALIGN(0x00000400)


/**
 * \defgroup        RINGBUFF Ring buffer
 * \brief           Generic ring buffer manager
 * \{
 */

/**
 * \brief           Enable buffer structure pointer parameter as volatile
 * To use this feature, uncomment keyword below
 */
#define RINGBUFF_VOLATILE                       volatile

/**
 * \brief           Adds 2 magic words to make sure if memory is corrupted
 *                  application can detect wrong data pointer and maximum size
 */
//#define RINGBUFF_USE_MAGIC                      1

/**
 * \brief           Event type for buffer operations
 */
typedef enum {
    RINGBUFF_EVT_READ,                          /*!< Read event */
    RINGBUFF_EVT_WRITE,                         /*!< Write event */
    RINGBUFF_EVT_RESET,                         /*!< Reset event */
} ringbuff_evt_type_t;

/**
 * \brief           Buffer structure forward declaration
 */
struct ringbuff;

/**
 * \brief           Event callback function type
 * \param[in]       buff: Buffer handle for event
 * \param[in]       evt: Event type
 * \param[in]       bp: Number of bytes written or read (when used), depends on event type
 */
typedef void (*ringbuff_evt_fn)(RINGBUFF_VOLATILE struct ringbuff* buff, ringbuff_evt_type_t evt, size_t bp);

/**
 * \brief           Buffer structure
 */
typedef struct ringbuff {
#if RINGBUFF_USE_MAGIC
    uint32_t magic1;                            /*!< Magic 1 word */
#endif /* RINGBUFF_USE_MAGIC */
    uint8_t* buff;                              /*!< Pointer to buffer data.
                                                    Buffer is considered initialized when `buff != NULL` and `size > 0` */
    size_t size;                                /*!< Size of buffer data. Size of actual buffer is `1` byte less than value holds */
    size_t r;                                   /*!< Next read pointer. Buffer is considered empty when `r == w` and full when `w == r - 1` */
    size_t w;                                   /*!< Next write pointer. Buffer is considered empty when `r == w` and full when `w == r - 1` */
    ringbuff_evt_fn evt_fn;                     /*!< Pointer to event callback function */
#if RINGBUFF_USE_MAGIC
    uint32_t magic2;                            /*!< Magic 2 word */
#endif /* RINGBUFF_USE_MAGIC */
} ringbuff_t;

uint8_t     ringbuff_init(RINGBUFF_VOLATILE ringbuff_t* buff, void* buffdata, size_t size);
uint8_t     ringbuff_is_ready(RINGBUFF_VOLATILE ringbuff_t* buff);
void        ringbuff_free(RINGBUFF_VOLATILE ringbuff_t* buff);
void        ringbuff_reset(RINGBUFF_VOLATILE ringbuff_t* buff);
void        ringbuff_set_evt_fn(RINGBUFF_VOLATILE ringbuff_t* buff, ringbuff_evt_fn fn);

/* Read/Write functions */
size_t      ringbuff_write(RINGBUFF_VOLATILE ringbuff_t* buff, const void* data, size_t btw);
size_t      ringbuff_read(RINGBUFF_VOLATILE ringbuff_t* buff, void* data, size_t btr);
size_t      ringbuff_peek(RINGBUFF_VOLATILE ringbuff_t* buff, size_t skip_count, void* data, size_t btp);

/* Buffer size information */
size_t      ringbuff_get_free(RINGBUFF_VOLATILE ringbuff_t* buff);
size_t      ringbuff_get_full(RINGBUFF_VOLATILE ringbuff_t* buff);

/* Read data block management */
void *      ringbuff_get_linear_block_read_address(RINGBUFF_VOLATILE ringbuff_t* buff);
size_t      ringbuff_get_linear_block_read_length(RINGBUFF_VOLATILE ringbuff_t* buff);
size_t      ringbuff_skip(RINGBUFF_VOLATILE ringbuff_t* buff, size_t len);

/* Write data block management */
void *      ringbuff_get_linear_block_write_address(RINGBUFF_VOLATILE ringbuff_t* buff);
size_t      ringbuff_get_linear_block_write_length(RINGBUFF_VOLATILE ringbuff_t* buff);
size_t      ringbuff_advance(RINGBUFF_VOLATILE ringbuff_t* buff, size_t len);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* SYSTEM_IPC_RING_BUFFER_H_ */
