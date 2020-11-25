#include "main.h"

static stepper_t stepper;
static uint8_t buffer[240];

void __attribute__ (( noreturn )) main(void) {
    usart_init();

    stepper.port = &PORTD;
    stepper.ddr = &DDRD;
    stepper.dir_pin = 5;
    stepper.enable_pin = 4;
    stepper.step_pin = 6;
    stepper.endstop_ddr = &PORTD;
    stepper.endstop_mask = PD3;
    stepper.endstop_pin = &PIND;
    stepper_init(&stepper);
    stepper_enable_auto(&stepper, 550, 1200);

    for(;;) {
        command_usart_read(buffer);

        command_packet_t *cmd = (command_type_t *) buffer;
        switch (cmd->hdr.type)
        {
            /*******************************
             * Handles Enable/Disable of motors
             *******************************/
            case COMMAND_TYPE_ENABLE:
                switch (cmd->body.payload[0])
                {
                    case COMMAND_MOTOR_MR:
                        stepper_enable(&stepper);
                        break;
                    case COMMAND_MOTOR_MX:
                        break;
                }
                break;
            case COMMAND_TYPE_DISABLE:
                switch (cmd->body.payload[0])
                {
                    case COMMAND_MOTOR_MR:
                        stepper_disable(&stepper);
                        break;
                    case COMMAND_MOTOR_MX:
                        break;
                }
                break;
            /*******************************
             * Handles Movement of motors
             *******************************/
            case COMMAND_TYPE_MOVE:
            {
                uint32_t pos = cmd->body.payload[1];
                switch (cmd->body.payload[0])
                {
                    case COMMAND_MOTOR_MR:

                        for (;;)printf("%d\r\n", pos);
                        // stepper_auto_set_target(&stepper, pos);
                        break;
                    case COMMAND_MOTOR_MX:
                        break;
                }
                break;
            }
        }
    }
    /*
    stepper_move_to(&stepper, 400, 800, 2000);

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
    stepper.endstop_ddr = &PORTD;
    stepper.endstop_mask = PD3;
    stepper.endstop_pin = &PIND;
    stepper_init(&stepper);

    stepper1.ddr = &DDRB;
    stepper1.port = &PORTB;
    stepper1.step_pin = 0;
    stepper1.enable_pin = 1;
    stepper1.dir_pin = 2;
    stepper1.endstop_ddr = &PORTD;
    stepper1.endstop_mask = PD2;
    stepper1.endstop_pin = &PIND;
    stepper_init(&stepper1);

    stepper_auto_home(&stepper1);
    stepper_auto_home(&stepper);

    stepper_enable_auto(&stepper1, 550, 6500);
    stepper_enable_auto(&stepper, 550, 6500);

    joystick_return_t prev;
    for (;;) {
        joystick_return_t m = joystick_read(&joystick);

        if (m.clicked) {
            stepper_auto_set_target(&stepper, 8000);
            stepper_auto_set_target(&stepper1, 8000);
            _delay_ms(100);
            continue;
        }
        
        uint16_t x_diff, y_diff;
        if (prev.x > m.x) x_diff = prev.x - m.x;
        else x_diff = m.x - prev.x;

        if (prev.y > m.y) y_diff = prev.y - m.y;
        else y_diff = m.y - prev.y;

        if (x_diff > 10) stepper_auto_set_target(&stepper, m.x);
        if (y_diff > 10) stepper_auto_set_target(&stepper1, m.y);

        prev = m;
        _delay_ms(10);
    }
    */
}
