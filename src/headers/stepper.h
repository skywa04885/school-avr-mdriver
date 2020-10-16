#ifndef _SRC_HEADERS_STEPPER_H
#define _SRC_HEADERS_STEPPER_H

#include "include.h"
#include "io.h"

typedef enum {
    STEPPER_DIR_FORWARD,
    STEPPER_DIR_BACKWARD
} stepper_dir_t;

typedef struct {
    uint8_t dir_pin, step_pin, enable_pin;
    volatile uint8_t *ddr, port;
} stepper_t;

void stepper_init(stepper_t *stepper);
void stepper_step(stepper_t *stepper, uint16_t delay);
void stepper_enable(stepper_t *stepper);
void stepper_disable(stepper_t *stepper);
void stepper_set_dir(stepper_t *stepper, stepper_dir_t dir);

#endif