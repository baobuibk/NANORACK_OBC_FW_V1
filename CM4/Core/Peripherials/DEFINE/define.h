/*
 * define.h
 *
 *  Created on: Nov 28, 2024
 *      Author: CAO HIEU
 */

#ifndef PERIPHERIALS_DEFINE_H_
#define PERIPHERIALS_DEFINE_H_

typedef enum
{
  Status_OK       = 0x00U,
  Status_ERROR    = 0x01U,
  Status_BUSY     = 0x02U,
  Status_TIMEOUT  = 0x03U
} Peripheral_StatusTypeDef;

#endif /* PERIPHERIALS_DEFINE_H_ */
