/*
 * serial.c
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#include "serial.h"

typedef struct {
    int pos;
    int len;
    char data[128];
} uart_buf_t;

static volatile uart_buf_t tx_buf;
static volatile uart_buf_t rx_buf;
static USART_TypeDef *usart;

void serial_init(USART_TypeDef *_usart) {
    usart = _usart;
    LL_USART_EnableIT_RXNE(usart);
}

void serial_print(const char *msg) {
    int len = strlen(msg);
    len = len > sizeof(tx_buf.data) ? sizeof(tx_buf.data) : len;
    memcpy(tx_buf.data, msg, len);
    tx_buf.len = len;
    tx_buf.pos = 0;
    LL_USART_EnableIT_TXE(usart);
    while (LL_USART_IsEnabledIT_TXE(usart)) {};
}

int serial_available(void) {
    return rx_buf.pos;
}

int serial_read_bytes(char *buf, int max_len) {
    int len = rx_buf.pos > max_len ? max_len : rx_buf.pos;
    memcpy(buf, rx_buf.data, len);
    rx_buf.pos = 0;
    return len;
}

void serial_tx_callback(void) {
    if (LL_USART_IsEnabledIT_TXE(usart) && LL_USART_IsActiveFlag_TXE(usart)) {
        uint8_t byte = tx_buf.data[tx_buf.pos++];
        LL_USART_TransmitData8(usart, byte);

        if (tx_buf.pos >= tx_buf.len) {
            LL_USART_DisableIT_TXE(usart);
        }
    }
}

void serial_rx_callback(void) {
    if (LL_USART_IsEnabledIT_RXNE(usart) && LL_USART_IsActiveFlag_RXNE(usart)) {
        uint8_t byte = LL_USART_ReceiveData8(usart);
        if (rx_buf.pos < sizeof(rx_buf.data)) {
            rx_buf.data[rx_buf.pos++] = byte;
        }
    }
}

