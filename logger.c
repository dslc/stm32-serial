/*
 * logger.c
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#include "logger.h"

typedef struct {
    int pos;
    int len;
    char data[128];
} uart_buf_t;

static volatile uart_buf_t tx_buf;
static USART_TypeDef *usart;

void log_init(USART_TypeDef *_usart) {
    usart = _usart;
}

void log_message(const char *msg) {
    int len = strlen(msg);
    len = len > sizeof(tx_buf.data) ? sizeof(tx_buf.data) : len;
    memcpy(tx_buf.data, msg, len);
    tx_buf.len = len;
    tx_buf.pos = 0;
    LL_USART_EnableIT_TXE(usart);
    while (LL_USART_IsEnabledIT_TXE(usart)) {};
}

void log_tx_callback(void) {
    if (LL_USART_IsEnabledIT_TXE(usart) && LL_USART_IsActiveFlag_TXE(usart)) {
        uint8_t byte = tx_buf.data[tx_buf.pos++];
        LL_USART_TransmitData8(usart, byte);

        if (tx_buf.pos >= tx_buf.len) {
            LL_USART_DisableIT_TXE(usart);
        }
    }
}

