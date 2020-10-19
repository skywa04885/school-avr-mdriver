#include "../headers/stepper.h"

static volatile uint32_t __m0_target_pos = 0, __m0_new_target_pos = UINT32_MAX, __m0_steps = 0, __m0_step = 0,
	__m0_half_of_steps = 0, __m0_down_at = 0, __m0_max_sps = 0, __m0_min_sps = 0, __m0_sps;
static volatile stepper_dir_t __m0_dir = STEPPER_DIR_FORWARD;
static volatile uint16_t __m0_idle_timer = 0;
static volatile bool __m0_idle = true;
static volatile stepper_t *__m0 = NULL;

ISR(TIMER0_COMPA_vect) {
	if (__m0 == NULL) return;

	if (__m0_target_pos == __m0->pos) {
		if (__m0_new_target_pos != UINT32_MAX) {
			// Sets the stepper direction, as variable and the real pin dir
			if (__m0_new_target_pos > __m0->pos) __m0_dir = STEPPER_DIR_FORWARD;
			else __m0_dir = STEPPER_DIR_BACKWARD;
			stepper_set_dir(__m0, __m0_dir);

			// Calculates the number of steps, and the half of the total steps
			//  this will be used for speedup  / slowdown
			if (__m0_new_target_pos > __m0->pos) __m0_steps = __m0_new_target_pos - __m0->pos;
			else __m0_steps = __m0->pos - __m0_new_target_pos;
			__m0_half_of_steps = __m0_steps / 2;

			// Calculates the difference between the max and min, divides it by two
			//  to get the steps it takes to slow down, if this is larger than the target
			//  make the slowdown happen at half of the movement
			__m0_down_at = (__m0_max_sps - __m0_min_sps) / 2;
			if (__m0_down_at > __m0_steps) __m0_down_at = __m0_half_of_steps;
			else __m0_down_at = __m0_steps - __m0_down_at;

			// Sets the movement state variables
			__m0_step = 0;
			__m0_target_pos = __m0_new_target_pos;
			__m0_new_target_pos = UINT32_MAX;
			__m0_idle_timer = 0;
			__m0_sps = __m0_min_sps;

			// Makes the motor enabled if not
			if (__m0_idle == true) {
				stepper_enable(__m0);
				__m0_idle = false;
			}
		} else if (++__m0_idle_timer > 4000 && __m0_idle == false) {
			stepper_disable(__m0);
			__m0_idle = true;
		}

		return;
	}

	// Calculates the new clock
	uint64_t next_cycle = F_CPU / 1024;		// 1024 of the clock
	next_cycle /= __m0_sps;
	OCR0A = next_cycle;

	// Sends the pulse to the driver to tell the driver that the motor must
	//  perform an step
	io_low(__m0->port, __m0->step_pin);
	_delay_us(1);
	io_high(__m0->port, __m0->step_pin);

	// Checks if we should increment or decrement the sps
	if (__m0_sps <= __m0_max_sps && __m0_step < __m0_half_of_steps) __m0_sps += 2;
	else if (__m0_sps > __m0_min_sps && __m0_step > __m0_half_of_steps && __m0_step >= __m0_down_at) __m0_sps -= 2;

	// Updates the current position, and updates the step
	//  so we can calculate when to slowdown and speedup
	if (__m0_dir == STEPPER_DIR_FORWARD) ++__m0->pos;
	else --__m0->pos;

	++__m0_step;
}

void stepper_enable_auto(stepper_t *stepper, uint32_t min_sps, uint32_t max_sps) {
	__m0_min_sps = min_sps;
	__m0_max_sps = max_sps;

	__m0_target_pos = 0;
	__m0 = stepper;

	TCCR0A = (1 << WGM01) | (1 << WGM02);		// Mode: CTC
	TCCR0B = (1 << CS00) | (1 << CS02); 		// Clock: 1/1024
	TIMSK0 = (1 << OCIE0A);									// Enable A match interrupt
	OCR0A = 15;

	sei();
}

void stepper_auto_set_target(stepper_t *stepper, uint32_t target) {
	__m0_new_target_pos = target;
}

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
		// Calculates the delay of the current step in microseconds
		//  after which we divide it by two
		uint32_t delay = (1000000 / steps_per_second) / 2;

		// Performs an single step
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
