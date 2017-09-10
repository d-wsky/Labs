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
 * tone.c
 *
 * Created: 01-Nov-16 23:04:17
 *  Author: Denis Vasilkovskii
 */ 

#include "tone.h"
#include "libtime.h"
#include "utils.h"
#include <stdbool.h>
#include <avr/interrupt.h>

#define WGM_MODE_NORMAL        ((0 << WGM21) | (0 << WGM20))
#define WGM_MODE_PH_COR_PWM    ((0 << WGM21) | (1 << WGM20))
#define WGM_MODE_CTC           ((1 << WGM21) | (0 << WGM20))
#define WGM_MODE_FAST_PWM      ((1 << WGM21) | (1 << WGM20))

#define COM_MODE_NORMAL        ((0 << COM21) | (0 << COM20))
#define COM_MODE_TOGGLE_OC2    ((0 << COM21) | (1 << COM20))
#define COM_MODE_CLEAR_OC2     ((1 << COM21) | (0 << COM20))
#define COM_MODE_SET_OC2       ((1 << COM21) | (1 << COM20))

#define HW_TIMER_STOP          0xF8
#define PS_MODE_OFF            ((0 << CS22) | (0 << CS21) | (0 << CS20))
#define PS_MODE_DIV1           ((0 << CS22) | (0 << CS21) | (1 << CS20))
#define PS_MODE_DIV8           ((0 << CS22) | (1 << CS21) | (0 << CS20))
#define PS_MODE_DIV64          ((0 << CS22) | (1 << CS21) | (1 << CS20))
#define PS_MODE_DIV256         ((1 << CS22) | (0 << CS21) | (0 << CS20))
#define PS_MODE_DIV1024        ((1 << CS22) | (0 << CS21) | (1 << CS20))
#define PS_MODE_EXT_FALLING    ((1 << CS22) | (1 << CS21) | (0 << CS20))
#define PS_MODE_EXT_RISING     ((1 << CS22) | (1 << CS21) | (1 << CS20))

#define OCR_IRQ_ENABLE         (1 << OCIE2)
#define OCR_IRQ_FLAG           (1 << OCF2)
#define OVF_IRQ_ENABLE         (1 << TOIE2)
#define OVF_IRQ_FLAG           (1 << TOV2)

//static const Gpio_t UNUSED_PIN = (const Gpio_t){.port = GPIO_PORTS_NUM, .pin = GPIO_PIN_ALL};
#define UNUSED_PIN (const Gpio_t){.port = GPIO_PORTS_NUM}
static const uint32_t HW_TIMER_MAX = 254;
static volatile Gpio_t used_pin_;
static timer_t * ms_timer_ptr_;

static inline bool IsPlaying() {
	return used_pin_.port != GPIO_PORTS_NUM;
}

static void timer_callback() {
	noTone(used_pin_);
}

ISR(TIMER2_COMP_vect) {
    if (IsPlaying()) {
		gpioPinToggle(used_pin_);
	}
}

void initTone() {
    timer_status_t status = timer_get(&ms_timer_ptr_);
    assert(status == TIMER_STATUS_OK);
    
    timer_set_periodic(ms_timer_ptr_, false);
    timer_set_callback(ms_timer_ptr_, timer_callback);
    
    used_pin_ = UNUSED_PIN;
    
    TCCR2 = WGM_MODE_CTC | COM_MODE_NORMAL | PS_MODE_OFF;
    TIMSK |= OCR_IRQ_ENABLE;
}

static void SetFrequency(uint16_t frequency) {
    const uint32_t PRESCALER[] = {1, 8, 64, 256, 1024};
    uint32_t top_integer;
    uint8_t ps_index;
    for (ps_index = 0; ps_index < ARRAY_SIZE(PRESCALER); ps_index++) {
        top_integer = F_CPU / (frequency * 2 * PRESCALER[ps_index]) - 1;

        if (top_integer <= HW_TIMER_MAX) {
            break;
        }
    }
    top_integer = LABLIB_MIN(top_integer, HW_TIMER_MAX);
    
    OCR2 = top_integer;
    TCCR2 &= HW_TIMER_STOP;
    TCCR2 |= ps_index + 1; 
}

static void SetDuration(uint32_t duration) {
	timer_stop(ms_timer_ptr_);
    if (duration != TONE_LENGTH_INFINITE) {
        timer_set_interval(ms_timer_ptr_, duration);
        timer_status_t status = timer_start(ms_timer_ptr_);
		assert(status == TIMER_STATUS_OK);
    }
}

static inline bool SamePin(const Gpio_t lhs, const Gpio_t rhs) {
    return lhs.pin == rhs.pin && lhs.port == rhs.port;
}

void tone(Gpio_t pin, uint16_t frequency, uint32_t duration) {
    if (!IsPlaying()) {
        used_pin_ = pin;
        gpioPinModeSet(used_pin_, GPIO_MODE_OUT);
    }
    
    if (SamePin(pin, used_pin_)) {
        SetFrequency(frequency);
        SetDuration(duration);
    }
}

void noTone(Gpio_t pin) {
    if (SamePin(pin, used_pin_)) {
        TCCR2 &= PS_MODE_OFF;
        
        timer_stop(ms_timer_ptr_);
        gpioPinModeSet(used_pin_, GPIO_MODE_IN);
        used_pin_ = UNUSED_PIN;
    }
}
