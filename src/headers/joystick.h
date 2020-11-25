#ifndef _SRC_HEADERS_JOYSTICK_H
#define _SRC_HEADERS_JOYSTICK_H

#include "include.h"
#include "io.h"

typedef struct {
    uint8_t x_pin, y_pin, sw_pin;
    volatile uint8_t *xy_ddr, *sw_ddr, *sw_portin, *sw_port;
} joystick_t;

typedef struct {
    uint16_t x, y;
    bool clicked;
} joystick_return_t;

void joystick_init(joystick_t *joystick);
joystick_return_t joystick_read(joystick_t *joystick);

#endif
