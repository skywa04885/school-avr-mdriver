#include "../headers/include.h"

void _delay_us_runtime(uint16_t us) {
    for (uint16_t i = 0; i < us; ++i) _delay_us(1);
}