#include "../headers/stepper.h"

STEPPER_ISR_VARS(0);
STEPPER_ISR(TIMER0_COMPA_vect, OCR0A, __m0, __m0_target_pos, 
	__m0_new_target_pos, __m0_steps, __m0_step, __m0_half_of_steps, 
	__m0_down_at, __m0_max_sps, __m0_min_sps, __m0_sps, __m0_dir, 
	__m0_idle_timer, __m0_idle)

STEPPER_ISR_VARS(1);
STEPPER_ISR(TIMER2_COMPA_vect, OCR2A, __m1, __m1_target_pos, 
	__m1_new_target_pos, __m1_steps, __m1_step, __m1_half_of_steps, 
	__m1_down_at, __m1_max_sps, __m1_min_sps, __m1_sps, __m1_dir, 
	__m1_idle_timer, __m1_idle)

void stepper_enable_auto(stepper_t *stepper, uint32_t min_sps, uint32_t max_sps) {
	// if (stepper->using_isr != -1) return;
	
	if (!__m0_isr_used) {
		__m0_isr_used = true;
		stepper->using_isr = 0;

		__m0_min_sps = min_sps;
		__m0_max_sps = max_sps;

		__m0_target_pos = 0;
		__m0 = stepper;

		TCCR0A = (1 << WGM01) | (1 << WGM02);
		TCCR0B = (1 << CS00) | (1 << CS02);
		TIMSK0 = (1 << OCIE0A);
		OCR0A = 15;
	} else if (!__m1_isr_used) {
		__m1_isr_used = true;
		stepper->using_isr = 1;

		__m1_min_sps = min_sps;
		__m1_max_sps = max_sps;

		__m1_target_pos = 0;
		__m1 = stepper;

		TCCR2A = (1 << WGM21);
		TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
		TIMSK2 = (1 << OCIE2A);
		OCR2A = 15;
	} else fprintf(stderr, "stepper_enable_auto() err: no ISR slot available\r\n");

	sei();
}

void stepper_auto_set_target(stepper_t *stepper, uint32_t target) {
	if (stepper->using_isr == -1) return;

	switch (stepper->using_isr) {
		case 0:
			__m0_new_target_pos = target;
			break;
		case 1:
			__m1_new_target_pos = target;
			break;
		default: return;
	}
}

bool stepper_is_at_dest(stepper_t *stepper) {
	if (stepper->using_isr == -1) return false;
	uint32_t target = 0;

	switch (stepper->using_isr) {
		case 0:
			target = __m0_target_pos;
			break;
		case 1:
			target = __m1_target_pos;
			break;
		default: return;
	}

	return (target == stepper->pos);
}

void stepper_init(stepper_t *stepper) {
	stepper->pos = 0;
	stepper->using_isr = -1;

	io_mode(stepper->ddr, stepper->dir_pin, IO_OUTPUT);
	io_mode(stepper->ddr, stepper->enable_pin, IO_OUTPUT);
	io_mode(stepper->ddr, stepper->step_pin, IO_OUTPUT);

	io_mode(stepper->endstop_ddr, stepper->endstop_mask, IO_INPUT);

	io_high(stepper->port, stepper->step_pin);
	stepper_disable(stepper);
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
		// Calculates the delay of the current step in microseconds
		//  after which we divide it by two
		uint32_t delay = (1000000 / steps_per_second);

		// Performs an single step
		io_low(stepper->port, stepper->step_pin);
		 _delay_us(1);
	 	io_high(stepper->port, stepper->step_pin);
		 
		// Waits for next pulse
		_delay_us_runtime(delay);

		if (steps_per_second <= max_sps && i < half_of_steps) steps_per_second += 2;
		else if (steps_per_second > min_sps && i > half_of_steps && i >= down_at) steps_per_second -= 2;
	}

	stepper->pos = target;
	return steps;
}

void stepper_auto_home(stepper_t *stepper)
{
	// Loops until we've hit the endstop
	stepper_enable(stepper);
	while (io_read(stepper->endstop_pin, stepper->endstop_mask)) {
		io_low(stepper->port, stepper->step_pin);
		_delay_us(1);
	 	io_high(stepper->port, stepper->step_pin);

		_delay_us(2400);
	}
	stepper_disable(stepper);

	stepper->pos = 0;
}