/*
 * serial.h
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "stm32f3xx_ll_usart.h"

void serial_init(USART_TypeDef *usart);
void serial_print(const char *msg);
void serial_tx_callback(void);

#endif /* SERIAL_H_ */
