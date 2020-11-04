#include "main.h"

static volatile stepper_t stepper, stepper1;
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
    joystick_init(&joystick);

    stepper.port = &PORTD;
    stepper.ddr = &DDRD;
    stepper.dir_pin = 7;
    stepper.enable_pin = 6;
    stepper.step_pin = 5;
    stepper_init(&stepper);
    stepper_enable_auto(&stepper, 550, 6050);

    stepper1.ddr = &DDRB;
    stepper1.port = &PORTB;
    stepper1.step_pin = 0;
    stepper1.enable_pin = 1;
    stepper1.dir_pin = 2;
    stepper_init(&stepper1);
    stepper_enable_auto(&stepper1, 550, 1050);

    joystick_return_t prev;
    for (;;) {
        joystick_return_t m = joystick_read(&joystick);
        
        uint16_t x_diff, y_diff;
        if (prev.x > m.x) x_diff = prev.x - m.x;
        else x_diff = m.x - prev.x;

        if (prev.y > m.y) y_diff = prev.y - m.y;
        else y_diff = m.y - prev.y;

        if (x_diff > 10) stepper_auto_set_target(&stepper, m.x);
        if (y_diff > 10) stepper_auto_set_target(&stepper1, m.y);

        prev = m;
        _delay_ms(100);
    }
}
