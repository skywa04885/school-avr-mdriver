#ifndef _SRC_HEADERS_UART_H
#define _SRC_HEADERS_UART_H

#include "include.h"

/******************************
 * Prototypes
 ******************************/

/**
 * Initializes USART
 */
void usart_init(void);

/**
 * Writes an char over USART
 * 
 * @param char
 */
void usart_write_char(char c);

/**
 * Reads an byte from usart
 * 
 * @return
 */
uint8_t usart_read_byte(void);

/**
 * Writes an byte to USART
 * 
 * @param byte
 */
void usart_write_byte(uint8_t b);

/**
 * Writes an string over USART
 * 
 * @param pointer
 */
void usart_write_string(const char *p);

/**
 * Writes an char to USART using the stream method
 * 
 * @param char
 * @param stream
 */
char usart_fp_write_char(char c, FILE *stream);

/******************************
 * Global Variables
 ******************************/

static FILE usart_stream = FDEV_SETUP_STREAM(usart_fp_write_char, NULL, _FDEV_SETUP_WRITE);

#endif
