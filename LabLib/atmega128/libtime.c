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

static void timer_pool_interval_notify();

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


struct timer_impl_t {
    struct {
        uint8_t  is_periodic: 1;
        uint8_t  is_started: 1;
    };
    uint32_t         interval_ms;
    uint32_t         count;
    timer_callback_t callback;
};

struct timer_impl_t timer_pool[MAX_SOFT_TIMER_AMOUNT];
typedef uint8_t timer_pool_it_t;

#if MAX_SOFT_TIMER_AMOUNT <= 8
typedef uint8_t timer_pool_map_t;
#elif MAX_SOFT_TIMER_AMOUNT <= 16
typedef uint16_t timer_pool_map_t;
#elif MAX_SOFT_TIMER_AMOUNT <= 32
typedef uint32_t timer_pool_map_t;
#elif MAX_SOFT_TIMER_AMOUNT <= 64
typedef uint64_t timer_pool_map_t;
#else
#error "MAX_SOFT_TIMER_AMOUNT must not exceed 64"
#endif

static timer_pool_map_t timer_pool_map_ = 0;

static inline bool is_timer_busy(timer_pool_it_t i) {
    return (timer_pool_map_ & (1 << i));
}

static inline void mark_timer_busy(timer_pool_it_t i) {
    timer_pool_map_ |=   1 << i;
}

static inline void mark_timer_free(timer_pool_it_t i) {
    timer_pool_map_ &= ~(1 << i);
}

static bool is_timer(timer_t * t) {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (t == &timer_pool[i]) {
            if (is_timer_busy(i)) {
                return true;
            }
        }
    }
    
    return false;
}

static timer_t * timer_pool_alloc() {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (!is_timer_busy(i)) {
            mark_timer_busy(i);
            return &timer_pool[i];
        }    
    }
    
    return NULL;
}

static void timer_pool_free(timer_t * t) {
    timer_pool_it_t i = (t - timer_pool) / sizeof(struct timer_impl_t);
    mark_timer_free(i);
}

static void timer_pool_interval_notify() {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (is_timer_busy(i)) {
            if (!timer_pool[i].is_started) {
                continue;
            }
            if (--timer_pool[i].count == 0) {
                assert(timer_pool[i].callback != NULL);
                timer_pool[i].callback();
                if (timer_pool[i].is_periodic) {
                    timer_pool[i].count = timer_pool[i].interval_ms;
                }
                else {
                    timer_pool[i].is_started = false;
                }
            }
        }
    }
}

timer_status_t timer_get(timer_t ** timer) {
    assert(timer != NULL);
    
    if ((*timer = timer_pool_alloc())) {
        return TIMER_STATUS_OK;
    }
    
    return TIMER_STATUS_NO_FREE_TIMERS;
}

timer_status_t timer_set_interval(timer_t * t, uint32_t interval) {
    if (!is_timer(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
	if (interval == 0) {
		return TIMER_STATUS_INCORRECT_INTERVAL;
	}
    
    t->interval_ms = interval;
    return TIMER_STATUS_OK;
}

timer_status_t timer_set_callback(struct timer_impl_t * t, timer_callback_t cb) {
    if (!is_timer(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->callback = cb;
    return TIMER_STATUS_OK;
}

timer_status_t timer_set_periodic(timer_t * t, bool is_periodic) {
    if (!is_timer(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->is_periodic = is_periodic;
    return TIMER_STATUS_OK;
}

timer_status_t timer_start(timer_t * t) {
    if (!is_timer(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    if (!t->callback) {
        return TIMER_STATUS_NO_CALLBACK;
    }
    
    if (!t->is_started) {
		if (t->interval_ms == 0) {
			return TIMER_STATUS_INCORRECT_INTERVAL;
		}
        t->count = t->interval_ms;
        t->is_started = true;
    }
    return TIMER_STATUS_OK;
}

timer_status_t timer_stop(timer_t * t) {
    if (!is_timer(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->is_started = false;
    t->count = t->interval_ms;
    return TIMER_STATUS_OK;
}

timer_status_t timer_release(timer_t * timer) {
    if (!is_timer(timer)) {
        return TIMER_STATUS_NOT_FOUND;
    }

    timer_pool_free(timer);
    return TIMER_STATUS_OK;
}
