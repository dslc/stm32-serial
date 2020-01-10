/*
 * logger.c
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#include "logger.h"

#include "stm32f3xx_ll_usart.h"

#define LOG_UART USART2

volatile uart_buf_t tx_buf;

void log_message(const char *msg) {
    int len = strlen(msg);
    len = len > sizeof(tx_buf.data) ? sizeof(tx_buf.data) : len;
    memcpy(tx_buf.data, msg, len);
    tx_buf.len = len;
    tx_buf.pos = 0;
    LL_USART_EnableIT_TXE(LOG_UART);
}

void log_tx_callback(void) {
    if (LL_USART_IsEnabledIT_TXE(LOG_UART) && LL_USART_IsActiveFlag_TXE(LOG_UART)) {
        uint8_t byte = tx_buf.data[tx_buf.pos++];
        LL_USART_TransmitData8(LOG_UART, byte);

        if (tx_buf.pos >= tx_buf.len) {
            LL_USART_DisableIT_TXE(LOG_UART);
        }
    }
}

