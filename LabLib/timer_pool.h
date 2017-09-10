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
 * timer_pool.h
 *
 * Created: 10-Sep-17 19:08:51
 *  Author: Denis Vasilkovskii
 */ 


#ifndef TIMER_POOL_H_
#define TIMER_POOL_H_

#include <stdbool.h>

#include "softtimer.h"

/*! \file timer_pool.h

\brief Модуль менджмента софтверных таймеров. Не предназначен для использования никем,
кроме явно указанных модулей.

Единственная функция, предназначенная для настройки пользователем - количество
софтверных таймеров. Регулируется с помощью макроса MAX_SOFT_TIMER_AMOUNT,
значение которого должно быть известно на этапе компиляции.
*/

typedef enum {
    TIMER_POOL_STATUS_OK,
    TIMER_POOL_STATUS_NOT_FOUND,
} timer_pool_status_t;


/*! \brief Внутренняя функция модуля Timer Pool. Не предназначена для
использования другими модулями, кроме softtimer.
*/
softtimer_t * timer_pool_alloc_timer();

/*! \brief Внутренняя функция модуля Timer Pool. Не предназначена для
использования другими модулями, кроме softtimer.
*/
void timer_pool_free_timer(softtimer_t * t);

/*! \brief Внутренняя функция модуля Timer Pool. Не предназначена для
использования другими модулями, кроме softtimer.
*/
bool timer_pool_is_ptr_valid(softtimer_t * t);

/*! \brief Внутренняя функция модуля Timer Pool. Не предназначена для
использования другими модулями, кроме libtime.
*/
void timer_pool_interval_notify();

#endif /* TIMER_POOL_H_ */
