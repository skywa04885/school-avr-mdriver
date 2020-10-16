#include "../headers/joystick.h"

void joystick_init(joystick_t *joystick) {
    io_mode(joystick->xy_ddr, joystick->x_pin, IO_INPUT);
    io_mode(joystick->xy_ddr, joystick->y_pin, IO_INPUT);
    io_mode(joystick->sw_ddr, joystick->sw_pin, IO_INPUT);

    io_high(joystick->sw_port, joystick->sw_pin);
}

joystick_return_t joystick_read(joystick_t *joystick) {
    joystick_return_t res;

    res.x = io_analog_read(joystick->x_pin);
    res.y = io_analog_read(joystick->y_pin);
    res.clicked = !io_read(joystick->sw_portin, joystick->sw_pin);

    return res;
}