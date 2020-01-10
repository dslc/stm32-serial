/*
 * logger.h
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#ifndef LOGGER_H_
#define LOGGER_H_

typedef struct {
	int pos;
	int len;
	char data[128];
} uart_buf_t;

void log_message(const char *msg);
void log_tx_callback(void);

#endif /* LOGGER_H_ */
