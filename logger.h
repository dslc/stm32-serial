/*
 * logger.h
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stm32f3xx_ll_usart.h"

void log_init(USART_TypeDef *usart);
void log_message(const char *msg);
void log_tx_callback(void);

#endif /* LOGGER_H_ */
