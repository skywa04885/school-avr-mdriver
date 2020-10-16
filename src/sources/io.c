#include "../headers/io.h"

void io_mode(volatile uint8_t *ddr, uint8_t pin, io_mode_t mode) {
    if (mode == IO_INPUT) *ddr &= ~(1 << pin);
    else if (mode == IO_OUTPUT) *ddr |= (1 << pin);
}

void io_analog_init(void) {
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t io_analog_read(uint8_t channel) {
    ADMUX = (1 << REFS0) | channel;
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    return ADC;
}

bool io_read(volatile uint8_t *portin, uint8_t pin) {
    return *portin & (1 << pin);
}

void io_high(volatile uint8_t *port, uint8_t pin) {
    *port |= (1 << pin);
}

void io_low(volatile uint8_t *port, uint8_t pin) {
    *port &= ~(1 << pin);
}