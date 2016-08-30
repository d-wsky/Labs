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
 * assert.c
 *
 * Created: 30-Aug-16 23:59:57
 *  Author: Denis Vasilkovskii
 */ 

#ifdef DEBUG

#include <assert.h>
#include <avr/interrupt.h>
#include "assert_transport.h"

#ifdef ASSERT_TRANSPORT_LCD
assert_transport_t * assert_transport = &lcd_assert_transport;
#elif defined(ASSERT_TRANSPORT_SERIAL)
#error This transport is not implemented yet
assert_transport_t * assert_transport = &serial_assert_transport;
#else
#warning No assert transport has been selected
#endif

void __assert(const char *__func, const char *__file, int __lineno, 
              const char *__sexp) {
    cli();
    assert_transport->init();
    assert_transport->print(__func, __file, __lineno, __sexp);
    
    while (1) {
        // дальнейшее исполнение программы приостановлено
    };
}

#endif // DEBUG