/*
 * serial.h
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#ifndef SERIAL_H_
#define SERIAL_H_

// Include the main.h header to ensure the macro for the relevant MCU has been defined
#include "main.h"

#ifdef STM32L1
#include "stm32l1xx_ll_usart.h"
#elif defined STM32F3
#include "stm32f3xx_ll_usart.h"
#endif

struct serial;

typedef struct serial serial_t;

typedef enum {
    SERIAL_LINE_ENDING_NONE,
    SERIAL_LINE_ENDING_CR,
    SERIAL_LINE_ENDING_LF,
    SERIAL_LINE_ENDING_CRLF
} serial_line_ending_t;

serial_t *serial_init(USART_TypeDef *usart);
void serial_set_baud_rate(serial_t *serial, uint32_t baud_rate);
void serial_print(serial_t *serial, const char *msg);
void serial_println(serial_t *serial, const char *msg);
void serial_set_line_ending(serial_t *serial, serial_line_ending_t ending);
int serial_read_bytes(serial_t *serial, char *buf, int max_len);
void serial_tx_callback(serial_t *serial);
void serial_rx_callback(serial_t *serial);
int serial_available(serial_t *serial);

#endif /* SERIAL_H_ */
