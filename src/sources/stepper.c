#include "../headers/stepper.h"

static volatile __motor0_target_pos = 0, __motor0_current_pos = 0;
static volatile stepper_t *__motor0 = NULL;

void stepper_init(stepper_t *stepper) {
	stepper->pos = 0;

	io_mode(stepper->ddr, stepper->dir_pin, IO_OUTPUT);
	io_mode(stepper->ddr, stepper->enable_pin, IO_OUTPUT);
	io_mode(stepper->ddr, stepper->step_pin, IO_OUTPUT);

	io_high(stepper->port, stepper->step_pin);
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

uint32_t stepper_move_to(stepper_t *stepper, uint32_t target, uint32_t min_sps, uint32_t max_sps) {
	// Checks in which direction we're going to move
	stepper_dir_t dir = (target > stepper->pos ? STEPPER_DIR_FORWARD : STEPPER_DIR_BACKWARD);
	stepper_set_dir(stepper, dir);

	// Calculates the number of steps
	uint32_t steps, half_of_steps, down_at;
	if (target > stepper->pos) steps = target - stepper->pos;
	else steps = stepper->pos - target;
	half_of_steps = steps / 2;

	// Calculates the difference between the max and min, and divides it by two, to get
	//  the steps it takes to slow down, if this is larger than the target, make it slowdown
	//  at the half, else make it steps - down_at
	down_at = ((max_sps - min_sps) / 2);
	if (down_at > steps) down_at = half_of_steps;
	else down_at = steps - down_at;

	// Starts stepping
	uint32_t steps_per_second = min_sps;
	for (uint32_t i = 0; i < steps; ++i) {
		uint32_t delay = (1000000 / steps_per_second) / 2;

		//printf("%d\n", delay);
		io_low(stepper->port, stepper->step_pin);
		 _delay_us_runtime(delay);
	 	io_high(stepper->port, stepper->step_pin);
		 _delay_us_runtime(delay);

		if (steps_per_second <= max_sps && i < half_of_steps) steps_per_second += 2;
		else if (steps_per_second > min_sps && i > half_of_steps && i >= down_at) steps_per_second -= 2;
	}

	stepper->pos = target;
	return steps;
}
