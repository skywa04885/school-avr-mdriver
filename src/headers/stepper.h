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
    int8_t *ddr, port;
		uint32_t pos;
} stepper_t;

void stepper_init(stepper_t *stepper);
void stepper_enable(stepper_t *stepper);
void stepper_disable(stepper_t *stepper);
void stepper_set_dir(stepper_t *stepper, stepper_dir_t dir);
uint32_t stepper_move_to(stepper_t *stepper, uint32_t target, uint32_t min_sps, uint32_t max_sps);

#endif
