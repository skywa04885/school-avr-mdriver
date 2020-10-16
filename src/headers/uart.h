#ifndef _SRC_HEADERS_UART_H
#define _SRC_HEADERS_UART_H

#include "include.h"

void uart_init(void);
void uart_write_char(char c);
void uart_write_stream_char(char c, FILE *stream);
void uart_write_string(const char *c);

char uart_read_char(void);
char uart_read_stream_char(FILE *stream);

static FILE uart_out = FDEV_SETUP_STREAM(uart_write_stream_char, NULL, _FDEV_SETUP_WRITE);
static FILE uart_in = FDEV_SETUP_STREAM(NULL, uart_read_stream_char, _FDEV_SETUP_READ);

#endif
