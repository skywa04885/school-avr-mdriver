#include "../headers/stepper.h"

void stepper_init(stepper_t *stepper) {
    io_mode(stepper->ddr, stepper->dir_pin, IO_OUTPUT);
    io_mode(stepper->ddr, stepper->enable_pin, IO_INPUT);
    io_mode(stepper->ddr, stepper->step_pin, IO_OUTPUT);
}

void stepper_step(stepper_t *stepper, uint16_t delay) {
    io_high(stepper->port, stepper->step_pin);
    _delay_us_runtime(delay);
    io_low(stepper->port, stepper->step_pin);
    _delay_us_runtime(delay);
}

void stepper_enable(stepper_t *stepper) {
    io_mode(stepper->ddr, stepper->enable_pin, IO_INPUT);
}

void stepper_disable(stepper_t *stepper) {
    io_mode(stepper->ddr, stepper->enable_pin, IO_OUTPUT);
}

void stepper_set_dir(stepper_t *stepper, stepper_dir_t dir) {
    if (dir == STEPPER_DIR_BACKWARD)
        io_high(stepper->port, stepper->dir_pin);
    else if (dir == STEPPER_DIR_FORWARD)
        io_low(stepper->port, stepper->dir_pin);
}
