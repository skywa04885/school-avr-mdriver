#include "main.h"

static volatile stepper_t stepper;
static volatile uint16_t delay = 120;

void __attribute__ ((noreturn)) main(void) {
    uart_init();

    io_analog_init();

    joystick_t joystick;
    joystick.sw_ddr = &DDRD;
    joystick.sw_pin = 4;
    joystick.sw_portin = &PIND;
    joystick.sw_port = &PORTD;

    joystick.x_pin = 0;
    joystick.y_pin = 1;
    joystick.xy_ddr = &DDRC;

    
    stepper.port = &PORTD;
    stepper.ddr = &DDRD;
    stepper.dir_pin = 7;
    stepper.enable_pin = 6;
    stepper.step_pin = 5;

    io_mode(&DDRB, 0, IO_OUTPUT);

    stepper_init(&stepper);
    stepper_enable_auto(&stepper, 550, 1850);

    LOGD("Main", "Stepper initialized");

    joystick_init(&joystick);
		LOGD("Main", "Joystick initialized");
    
    bool enabled = true;

    uint32_t prev_target_pos = 512;
    stepper_auto_set_target(&stepper, 820);
    _delay_ms(12);
    stepper_auto_set_target(&stepper, 0);
    for (;;) {
      joystick_return_t ret = joystick_read(&joystick);
      _delay_ms(20);
    }
}
