/* 
 * Copyright (c) 2016, Denis Vasilkovskii
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      - Neither the name of the OINPE NRNU MEPhI nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * interrupts.c
 *
 * Created: 10-Aug-16 00:54:02
 *  Author: Denis Vasilkovskii
 */ 

#include "interrupts.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

static Isr_t callbacks[IRQ_SOURCES_AMOUNT] = {NULL};

static inline void enableIrq(IrqPin_t pin, IrqMode_t mode);
static inline void disableIrq(IrqPin_t pin);
static const uint8_t MODE_MASK = 3;

ISR(INT0_vect) {
    if (callbacks[0] != NULL) {
        callbacks[0]();
    }
}

#define AUTOGEN_ISR(n)                        \
ISR(INT##n##_vect) {                      \
    if (callbacks[n] != NULL) {           \
        callbacks[n]();                   \
    }                                     \
}

AUTOGEN_ISR(1);
AUTOGEN_ISR(2);
AUTOGEN_ISR(3);
AUTOGEN_ISR(4);
AUTOGEN_ISR(5);
AUTOGEN_ISR(6);
AUTOGEN_ISR(7);

void interruptAttach(IrqPin_t pin, Isr_t isr, IrqMode_t mode) {
    // \todo assert(pin < IRQ_SOURCES_AMOUNT);
    // \todo assert(callbacks[pin] == NULL);
    callbacks[pin] = isr;
    enableIrq(pin, mode);    
}

static inline void enableIrq(IrqPin_t pin, IrqMode_t mode) {
    // \todo atomic_start();
    if (pin < IRQ_PIN_4) {
        EICRA &= ~(MODE_MASK << (pin * 2));
        EICRA |=   mode      << (pin * 2);
    }
    else {
        EICRA &= ~(MODE_MASK << ((pin - IRQ_PIN_4) * 2));
        EICRA |=   mode      << ((pin - IRQ_PIN_4) * 2);
    }
    EIFR   = _BV(pin);
    EIMSK |= _BV(pin);
    // \todo atomic_end();
}

void interruptDeattach(IrqPin_t pin) {
    // \todo assert(pin < IRQ_SOURCES_AMOUNT);
    // \todo assert(callbacks[pin] != NULL);
    disableIrq(pin);
    callbacks[pin] = NULL;
}

static inline void disableIrq(IrqPin_t pin) {
    // \todo atomic_start
    EIMSK |= _BV(pin);
    if (pin < IRQ_PIN_4) {
        EICRA &= ~(MODE_MASK << (pin * 2));
    }
    else {
        EICRA &= ~(MODE_MASK << ((pin - IRQ_PIN_4) * 2));
    }
    // \todo atomic_end
    EIFR   = _BV(pin);
}

void interruptsEnable() {
    sei();
}

void interruptsDisable() {
   cli();
}
