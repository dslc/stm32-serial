/*
 * serial.h
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "stm32f3xx_ll_usart.h"

struct serial;

typedef struct serial serial_t;

serial_t *serial_init(USART_TypeDef *usart);
void serial_print(const char *msg);
int serial_read_bytes(char *buf, int max_len);
void serial_tx_callback(void);
void serial_rx_callback(void);
int serial_available(void);

#endif /* SERIAL_H_ */
