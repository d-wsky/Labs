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
 * softtimer_private.h
 *
 * Created: 10-Sep-17 20:06:50
 *  Author: Denis Vasilkovskii
 */ 


#ifndef SOFTTIMER_PRIVATE_H_
#define SOFTTIMER_PRIVATE_H_

struct timer_impl_t {
    struct {
        uint8_t  is_periodic: 1;
        uint8_t  is_started: 1;
    };
    uint32_t interval_ms;
    uint32_t count;
    softtimer_callback_t callback;
};


/*!\brief Внутренняя функция модуля таймера. */
#define softtimer_timer_size()   (sizeof(struct timer_impl_t))

/*!\brief Внутренняя функция модуля таймера. */
void softtimer_tick(softtimer_t * t);


#endif /* SOFTTIMER_PRIVATE_H_ */
