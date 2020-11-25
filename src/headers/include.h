#ifndef _SRC_HEADERS_INCLUDE_H
#define _SRC_HEADERS_INCLUDE_H
/******************************
 * Processor definitions
 ******************************/

#define BAUD 9600
#define F_CPU 16000000UL

#define UBRR0_VALUE ((F_CPU / (16UL * BAUD)) - 1)

/******************************
 * GNU GCC Standard Library
 ******************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/******************************
 * AVR Standard Library
 ******************************/

#include <avr/io.h>
#include <avr/interrupt.h>

/******************************
 * Utilities
 ******************************/

#include <util/delay.h>

/******************************
 * Definitions
 ******************************/

#define LOGD(LABEL, FMT, ...) \
    printf("[%s]: " FMT "\n", LABEL, ## __VA_ARGS__)

/******************************
 * Prototypes
 ******************************/

void _delay_us_runtime(uint16_t us);

#endif
