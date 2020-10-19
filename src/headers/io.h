#ifndef _SRC_HEADERS_IO_H
#define _SRC_HEADERS_IO_H

#include "include.h"

typedef enum __attribute__ ((packed)) {
    IO_INPUT, IO_OUTPUT
} io_mode_t;

void io_mode(volatile uint8_t *ddr, uint8_t pin, io_mode_t mode);
void io_high(volatile uint8_t *port, uint8_t pin);
void io_low(volatile uint8_t *port, uint8_t pin);
void io_toggle(volatile uint8_t *port, uint8_t pin);
bool io_read(volatile uint8_t *portin, uint8_t pin);

void io_analog_init(void);
uint16_t io_analog_read(uint8_t channel);

#endif
