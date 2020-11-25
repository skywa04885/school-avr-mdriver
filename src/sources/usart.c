#include "../headers/usart.h"

/**
 * Initializes USART
 */
void usart_init(void) {
    UBRR0L = UBRR0_VALUE;
    UBRR0H = UBRR0_VALUE >> 8;

    UCSR0A = 0x00;                              /* Clear status register */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);       /* Enable RX and TX */
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);     /* 8 Bits per Byte */

    stdin = stdout = stderr = &usart_stream;    /* Redirect stdin,stdout,stderr */
}

/**
 * Writes an char over USART
 * 
 * @param char
 */
void usart_write_char(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

/**
 * Reads an byte from usart
 * 
 * @return
 */
uint8_t usart_read_byte(void)
{
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

/**
 * Writes an byte to USART
 * 
 * @param byte
 */
void usart_write_byte(uint8_t b)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = b;
}

/**
 * Writes an string over USART
 * 
 * @param pointer
 */
void usart_write_string(const char *p)
{
	while (*p != '\0') usart_write_char(*p++);
}

/**
 * Writes an char to USART using the stream method
 * 
 * @param char
 * @param stream
 */
char usart_fp_write_char(char c, FILE *stream)
{
	while ((UCSR0A & (1 << UDRE0)) == 0);
	UDR0 = c;
}
