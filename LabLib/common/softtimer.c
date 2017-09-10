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
 * softtimer.c
 *
 * Created: 10-Sep-17 19:28:29
 *  Author: Denis Vasilkovskii
 */ 

#include "timer_pool.h"
#include "softtimer_private.h"
#include <stdint.h>
#include <assert.h>

softtimer_t * softtimer_alloc() {
    return timer_pool_alloc_timer();
}

timer_status_t softtimer_set_interval(softtimer_t * t, uint32_t interval) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    if (interval == 0) {
        return TIMER_STATUS_INCORRECT_INTERVAL;
    }
    
    t->interval_ms = interval;
    return TIMER_STATUS_OK;
}

timer_status_t softtimer_set_callback(struct timer_impl_t * t, softtimer_callback_t cb) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->callback = cb;
    return TIMER_STATUS_OK;
}

timer_status_t softtimer_set_periodic(softtimer_t * t, bool is_periodic) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->is_periodic = is_periodic;
    return TIMER_STATUS_OK;
}

timer_status_t softtimer_start(softtimer_t * t) {
    if (!timer_pool_is_ptr_valid(t)) {
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

bool softtimer_is_started(softtimer_t * t) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    return t->is_started;
}

timer_status_t softtimer_stop(softtimer_t * t) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }
    
    t->is_started = false;
    t->count = t->interval_ms;
    return TIMER_STATUS_OK;
}

timer_status_t softtimer_free(softtimer_t * t) {
    if (!timer_pool_is_ptr_valid(t)) {
        return TIMER_STATUS_NOT_FOUND;
    }

    timer_pool_free_timer(t);
    return TIMER_STATUS_OK;
}

void softtimer_tick(softtimer_t * t) {
    assert(timer_pool_is_ptr_valid(t));
    
    if (--t->count == 0) {
        assert(t->callback != NULL);
        t->callback();
        if (t->is_periodic) {
            t->count = t->interval_ms;
        }
        else {
            t->is_started = false;
        }
    }
}
