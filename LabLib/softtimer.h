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
 * softtimer.h
 *
 * Created: 10-Sep-17 19:19:08
 *  Author: Denis Vasilkovskii
 */ 


#ifndef SOFTTIMER_H_
#define SOFTTIMER_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    TIMER_STATUS_OK,
    TIMER_STATUS_NOT_FOUND,
    TIMER_STATUS_NO_FREE_TIMERS,
    TIMER_STATUS_NO_CALLBACK,
    TIMER_STATUS_INCORRECT_INTERVAL,
} timer_status_t;

struct timer_impl_t;
typedef struct timer_impl_t softtimer_t;
typedef void (*softtimer_callback_t)();

softtimer_t * softtimer_alloc();
timer_status_t softtimer_set_interval(softtimer_t * t, uint32_t interval);
timer_status_t softtimer_set_callback(softtimer_t * t, softtimer_callback_t cb);
timer_status_t softtimer_set_periodic(softtimer_t * t, bool is_periodic);
timer_status_t softtimer_start(softtimer_t * t);
bool softtimer_is_started(softtimer_t * t);
timer_status_t softtimer_stop(softtimer_t * t);
timer_status_t softtimer_free(softtimer_t * t);

#endif /* SOFTTIMER_H_ */
