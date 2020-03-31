/*
 * serial.c
 *
 *  Created on: 10 Jan 2020
 *      Author: david
 */

#include "serial.h"
#include <string.h>

#ifndef SERIAL_TX_BUF_SIZE
#define SERIAL_TX_BUF_SIZE 128
#endif

#ifndef SERIAL_RX_BUF_SIZE
#define SERIAL_RX_BUF_SIZE 128
#endif

#ifndef SERIAL_N_INTERFACES
#define SERIAL_N_INTERFACES 2
#endif

typedef struct {
    int pos;
    int len;
    char data[SERIAL_RX_BUF_SIZE];
} serial_tx_buf_t;

typedef struct {
    int pos;
    char data[SERIAL_RX_BUF_SIZE];
} serial_rx_buf_t;

struct serial {
    volatile serial_tx_buf_t tx_buf;
    volatile serial_rx_buf_t rx_buf;
    USART_TypeDef *usart;
    serial_line_ending_t line_ending;
};

static serial_t ifs[SERIAL_N_INTERFACES];
static uint8_t next_if = 0; // next interface

serial_t *serial_init(USART_TypeDef *_usart) {
    if (next_if >= SERIAL_N_INTERFACES) {
        return NULL;
    }
    serial_t *s = &ifs[next_if++];
    s->usart = _usart;
    s->line_ending = SERIAL_LINE_ENDING_LF;
    LL_USART_EnableIT_RXNE(s->usart);

    return s;
}

static void on_tx_ready(serial_t *serial) {
    LL_USART_EnableIT_TXE(serial->usart);
    while (LL_USART_IsEnabledIT_TXE(serial->usart)) {};
}

void serial_print(serial_t *serial, const char *msg) {
    serial_tx_buf_t *buf = &(serial->tx_buf);
    int len = strlen(msg);
    len = len > sizeof(buf->data) ? sizeof(buf->data) : len;
    memcpy(buf->data, msg, len);
    buf->len = len;
    buf->pos = 0;
    on_tx_ready(serial);
}

void serial_println(serial_t *serial, const char *msg) {
    serial_print(serial, msg);
    switch (serial->line_ending) {
        case SERIAL_LINE_ENDING_LF:
            serial_print(serial, "\n");
            break;
        case SERIAL_LINE_ENDING_CR:
            serial_print(serial, "\r");
            break;
        case SERIAL_LINE_ENDING_CRLF:
            serial_print(serial, "\r\n");
            break;
        default:
            break;
    }
}

void serial_set_line_ending(serial_t *serial, serial_line_ending_t ending) {
    serial->line_ending = ending;
}

int serial_available(serial_t *serial) {
    return serial->rx_buf.pos;
}

int serial_read_bytes(serial_t *serial, char *dest, int max_len) {
    serial_rx_buf_t *buf = &(serial->rx_buf);
    int len = buf->pos > max_len ? max_len : buf->pos;
    memcpy(dest, buf->data, len);
    buf->pos = 0;
    return len;
}

void serial_tx_callback(serial_t *serial) {
    if (LL_USART_IsEnabledIT_TXE(serial->usart) && LL_USART_IsActiveFlag_TXE(serial->usart)) {
        serial_tx_buf_t *buf = &serial->tx_buf;
        uint8_t byte = buf->data[buf->pos++];
        LL_USART_TransmitData8(serial->usart, byte);

        if (buf->pos >= buf->len) {
            LL_USART_DisableIT_TXE(serial->usart);
        }
    }
}

void serial_rx_callback(serial_t *serial) {
    if (LL_USART_IsEnabledIT_RXNE(serial->usart) && LL_USART_IsActiveFlag_RXNE(serial->usart)) {
        serial_rx_buf_t *buf = &(serial->rx_buf);
        uint8_t byte = LL_USART_ReceiveData8(serial->usart);
        if (buf->pos < sizeof(buf->data)) {
            buf->data[buf->pos++] = byte;
        }
    }
}

