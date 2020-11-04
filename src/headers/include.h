#ifndef _SRC_HEADERS_INCLUDE_H
#define _SRC_HEADERS_INCLUDE_H

#ifndef BAUD
#define BAUD 9600
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <util/setbaud.h>

void _delay_us_runtime(uint16_t us);

#define LOGD(LABEL, FMT, ...) \
    printf("[%s]: " FMT "\n", LABEL, ## __VA_ARGS__)

#endif
