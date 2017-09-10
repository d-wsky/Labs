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
 * libtime.c
 *
 * Created: 12-Sep-16 23:45:39
 *  Author: Denis Vasilkovskii
 */ 

#include "libtime.h"
#include <avr/interrupt.h>
#include <stddef.h>
#include "assert.h"
#include "timer_pool.h"

#define WGM_MODE_NORMAL        ((0 << WGM21) | (0 << WGM20))
#define WGM_MODE_PH_COR_PWM    ((0 << WGM21) | (1 << WGM20))
#define WGM_MODE_CTC           ((1 << WGM21) | (0 << WGM20))
#define WGM_MODE_FAST_PWM      ((1 << WGM21) | (1 << WGM20))

#define COM_MODE_NORMAL        ((0 << COM21) | (0 << COM20))
#define COM_MODE_TOGGLE_OC2    ((0 << COM21) | (1 << COM20))
#define COM_MODE_CLEAR_OC2     ((1 << COM21) | (0 << COM20))
#define COM_MODE_SET_OC2       ((1 << COM21) | (1 << COM20))

#define PS_MODE_OFF            ((0 << CS22) | (0 << CS21) | (0 << CS20))
#define PS_MODE_DIV1           ((0 << CS22) | (0 << CS21) | (1 << CS20))
#define PS_MODE_DIV8           ((0 << CS22) | (1 << CS21) | (0 << CS20))
#define PS_MODE_DIV64          ((0 << CS22) | (1 << CS21) | (1 << CS20))
#define PS_MODE_DIV256         ((1 << CS22) | (0 << CS21) | (0 << CS20))
#define PS_MODE_DIV1024        ((1 << CS22) | (0 << CS21) | (1 << CS20))
#define PS_MODE_EXT_FALLING    ((1 << CS22) | (1 << CS21) | (0 << CS20))
#define PS_MODE_EXT_RISING     ((1 << CS22) | (1 << CS21) | (1 << CS20))

#define OCR_IRQ_ENABLE         (1 << OCIE3A)
#define OCR_IRQ_FLAG           (1 << OCF3A)
#define OVF_IRQ_ENABLE         (1 << TOIE3)
#define OVF_IRQ_FLAG           (1 << TOV3)

#define IRQ_INTERVAL           10000U

static uint32_t micros_ = 0;
static uint32_t millis_ = 0;

typedef uint16_t timer_wide_t;
#define TIMER_WIDE_MAX           UINT8_MAX


void initLibTime() {
    // выбираем режим CTC с верхним значением, определяемым
    // регистром OCR3A
    TCCR3A |= (0 << WGM30) | (0 << WGM31);
    TCCR3B |= (1 << WGM32) | (0 << WGM33);
    // выбираем частоту счета 1 тик
    TCCR3B |= (0 << CS32) | (0 << CS31) | (1 << CS30);
    // выбираем верхний предел счета
    OCR3A   = IRQ_INTERVAL;
    // этот регистр делится с другими таймерами.
    // кто знает, как их потом настроят?
    ETIMSK |= OCR_IRQ_ENABLE;
}

ISR(TIMER3_COMPA_vect) {
    micros_ += IRQ_INTERVAL;
    millis_++;
    
    timer_pool_interval_notify();
}

uint32_t micros() {
    // todo atomic
    uint32_t result = TCNT3;
    if (ETIFR & OCR_IRQ_FLAG) {
        result += micros_ + IRQ_INTERVAL;
    }
    else {
        result += micros_;
    }
    // todo end atomic
    return result;
}

uint32_t millis() {
    // todo atomic
    uint32_t result = millis_;
    // todo end atomic
    return result;
}
