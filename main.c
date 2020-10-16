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
    LOGD("Main", "Stepper initialized");

    joystick_init(&joystick);

    bool enabled = true;
    for (;;) {
        joystick_return_t ret = joystick_read(&joystick);
        printf("Joystick > { X: %-5d, Y: %-5d, SW: %s }\n", ret.x, ret.y, (ret.clicked == true ? "On" : "Off"));

        if (ret.clicked) {
            if (enabled) {
                enabled = false;
                stepper_disable(&stepper);
            } else {
                enabled = true;
                stepper_enable(&stepper);
            }

            _delay_ms(1000);
        }
    }
}