#include "../headers/uart.h"

void uart_init(void) {
    // Initializes the UART hardware, and sets the baud
    //  rate and size to 8 bits

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif

    UCSR0A |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    // Sets the file streams of stdout and stdin
    //  to the UART ones

    stdout = &uart_out;
    stdin = &uart_in;
}

void uart_write_char(char c) {
    if (c == '\n') uart_write_char('\r');
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void uart_write_stream_char(char c, FILE *stream) {
    if (c == '\n') uart_write_stream_char('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

void uart_write_string(const char *c) {
    while (*c != '\0') uart_write_char(*c++);
}

char uart_read_char(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return RXC0;
}

char uart_read_stream_char(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    uart_write_stream_char(UDR0, stream);
    return UDR0;
}