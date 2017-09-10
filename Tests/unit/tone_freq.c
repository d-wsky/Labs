#include "structure.h"
#include "utils.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

void setup() {
}

void loop() {
    static const uint32_t F_CPU = 10000000;
    static const uint32_t MAX = 254;
    static const uint32_t PRESCALER[] = {1, 8, 64, 256, 1024};
    size_t ps_index = 0;
    static const uint16_t START_FREQ = 1;
    static const uint16_t END_FREQ = 65535;

    printf("Expected freq,Top,Prescaler,Real freq,Error abs,Error relative\n");
    for (size_t f = START_FREQ; f < END_FREQ; f++) {
        uint32_t top_integer;
        for (ps_index = 0; ps_index < ARRAY_SIZE(PRESCALER); ps_index++) {
            top_integer = F_CPU/f/PRESCALER[ps_index];

            if (top_integer <= MAX) {
                break;
            }
        }
        double real_freq = (F_CPU * 1.)/(top_integer * 1.)/PRESCALER[ps_index];
        double error_freq_abs = real_freq - f;
        double error_freq_percnt = (real_freq - f) / f * 100.;

        if (ps_index == ARRAY_SIZE(PRESCALER)) {
            printf("%d,%d,%d,%s,%s,%s\n",
                   f, top_integer, PRESCALER[ps_index - 1], "----", "----", "----");
        }
        else {
            printf("%d,%d,%d,%1.1f,%1.1f,%1.3f\n",
                   f, top_integer, PRESCALER[ps_index], real_freq, error_freq_abs, error_freq_percnt);
        }
    }
    _Exit(0);
}
