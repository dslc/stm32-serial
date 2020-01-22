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
void serial_print(serial_t *serial, const char *msg);
void serial_println(serial_t *serial, const char *msg);
void serial_printrln(serial_t *serial, const char *msg);
int serial_read_bytes(serial_t *serial, char *buf, int max_len);
void serial_tx_callback(serial_t *serial);
void serial_rx_callback(serial_t *serial);
int serial_available(serial_t *serial);

#endif /* SERIAL_H_ */
