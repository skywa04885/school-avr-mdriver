#ifndef _SRC_HEADERS_STEPPER_H
#define _SRC_HEADERS_STEPPER_H

#include "include.h"
#include "io.h"

#define STEPPER_ISR_VARS(N) \
static volatile uint32_t __m##N##_target_pos = 0, __m##N##_new_target_pos = UINT32_MAX, __m##N##_steps = 0, __m##N##_step = 0, \
	__m##N##_half_of_steps = 0, __m##N##_down_at = 0, __m##N##_max_sps = 0, __m##N##_min_sps = 0, __m##N##_sps = 0; \
static volatile stepper_dir_t __m##N##_dir = STEPPER_DIR_FORWARD; \
static volatile uint16_t __m##N##_idle_timer = 0; \
static volatile bool __m##N##_idle = true, __m##N##_isr_used = false; \
static volatile stepper_t *__m##N = NULL; \

#define STEPPER_ISR(TIMER_VECT, OCRA, MOTOR, TARGET_POS, NEW_TARGET_POS, STEPS, STEP, HALF_OF_STEPS, DOWN_AT, MAX_SPS, MIN_SPS, SPS, DIR, IDLE_TIMER, IDLE) \
ISR(TIMER_VECT) { \
	if (MOTOR == NULL) return; \
\
	if (TARGET_POS == MOTOR->pos) { \
		if (NEW_TARGET_POS != UINT32_MAX) { \
			/* Sets the stepper direction, as variable and the real pin dir */ \
			if (NEW_TARGET_POS > MOTOR->pos) DIR = STEPPER_DIR_FORWARD; \
			else DIR = STEPPER_DIR_BACKWARD; \
			stepper_set_dir(MOTOR, DIR); \
\
			/* Calculates the number of steps, and the half of the total steps */ \
			/*  this will be used for speedup  / slowdown */ \
			if (NEW_TARGET_POS > MOTOR->pos) STEPS = NEW_TARGET_POS - MOTOR->pos; \
			else STEPS = MOTOR->pos - NEW_TARGET_POS; \
			HALF_OF_STEPS = STEPS / 2; \
\
			/* Calculates the difference between the max and min, divides it by two */ \
			/*  to get the steps it takes to slow down, if this is larger than the target */ \
			/*  make the slowdown happen at half of the movement */ \
			DOWN_AT = (MAX_SPS - MIN_SPS) / 2; \
			if (DOWN_AT > STEPS) DOWN_AT = HALF_OF_STEPS; \
			else DOWN_AT = STEPS - DOWN_AT; \
\
			/* Sets the movement state variables */ \
			STEP = 0; \
			TARGET_POS = NEW_TARGET_POS; \
			NEW_TARGET_POS = UINT32_MAX; \
			IDLE_TIMER = 0; \
			SPS = MIN_SPS; \
\
			/* Makes the motor enabled if not */ \
			if (IDLE == true) { \
				stepper_enable(MOTOR); \
				IDLE = false; \
			} \
		} else if (++IDLE_TIMER > 4000 && IDLE == false) { \
			stepper_disable(MOTOR); \
			IDLE = true; \
		} \
\
		return; \
	} \
\
	/* Calculates the new clock */ \
	uint64_t next_cycle = F_CPU / 1024;		/* 1024 of the clock */ \
	next_cycle /= SPS; \
	OCRA = next_cycle; \
\
	/* Sends the pulse to the driver to tell the driver that the motor must */ \
	/*  perform an step */ \
	io_low(MOTOR->port, MOTOR->step_pin); \
	_delay_us(1); \
	io_high(MOTOR->port, MOTOR->step_pin); \
\
	/* Checks if we should increment or decrement the sps */ \
	if (SPS <= MAX_SPS && STEP < HALF_OF_STEPS) SPS += 2; \
	else if (SPS > MIN_SPS && STEP > HALF_OF_STEPS && STEP >= DOWN_AT) SPS -= 2; \
\
	/* Updates the current position, and updates the step */ \
	/*  so we can calculate when to slowdown and speedup */ \
	if (DIR == STEPPER_DIR_FORWARD) ++MOTOR->pos; \
	else --MOTOR->pos; \
\
	++STEP; \
}

typedef enum {
    STEPPER_DIR_FORWARD,
    STEPPER_DIR_BACKWARD
} stepper_dir_t;

typedef struct {
    uint8_t dir_pin, step_pin, enable_pin;
    int8_t *ddr, port;
    uint32_t pos;
    int8_t using_isr;

		uint8_t *endstop_pin, *endstop_ddr;
		uint8_t endstop_mask;
} stepper_t;

void stepper_init(stepper_t *stepper);
void stepper_enable(stepper_t *stepper);
void stepper_disable(stepper_t *stepper);
void stepper_set_dir(stepper_t *stepper, stepper_dir_t dir);
bool stepper_is_at_dest(stepper_t *stepper);
uint32_t stepper_move_to(stepper_t *stepper, uint32_t target, 
		uint32_t min_sps, uint32_t max_sps);
void stepper_enable_auto(stepper_t *stepper, uint32_t min_sps, uint32_t max_sps);
void stepper_auto_set_target(stepper_t *stepper, uint32_t target);
void stepper_auto_home(stepper_t *stepper);

#endif
