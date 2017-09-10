/* 
 * Copyright (c) 2017, Denis Vasilkovskii
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
 * timer_pool.c
 *
 * Created: 10-Sep-17 19:07:00
 *  Author: Denis Vasilkovskii
 */

#include "timer_pool.h"
#include "softtimer_private.h"
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#ifndef MAX_SOFT_TIMER_AMOUNT
#define MAX_SOFT_TIMER_AMOUNT      8
#endif

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

uint8_t timer_pool[MAX_SOFT_TIMER_AMOUNT * softtimer_timer_size()];
typedef uint8_t timer_pool_it_t;

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

static softtimer_t * get_timer_at_index(uint8_t i) {
    assert(i < MAX_SOFT_TIMER_AMOUNT);
    return (softtimer_t *)&timer_pool[i * softtimer_timer_size()];
}

bool timer_pool_is_ptr_valid(softtimer_t * t) {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (t == get_timer_at_index(i)) {
            if (is_timer_busy(i)) {
                return true;
            }
        }
    }
    
    return false;
}

softtimer_t * timer_pool_alloc_timer() {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (!is_timer_busy(i)) {
            mark_timer_busy(i);
            return get_timer_at_index(i);
        }
    }
    
    return NULL;
}

void timer_pool_free_timer(softtimer_t * t) {
    assert(timer_pool_is_ptr_valid(t));
    timer_pool_it_t i = (t - ((softtimer_t *)timer_pool)) / softtimer_timer_size();
    mark_timer_free(i);
}

void timer_pool_interval_notify() {
    for (timer_pool_it_t i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++) {
        if (is_timer_busy(i)) {
            if (!softtimer_is_started(get_timer_at_index(i))) {
                continue;
            }
            softtimer_tick(get_timer_at_index(i));
        }
    }
}
