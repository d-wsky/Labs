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
 * libtime.h
 *
 * Created: 12-Sep-16 23:29:03
 *  Author: Denis Vasilkovskii
 */ 


#ifndef LABLIB_LIBTIME_H
#define LABLIB_LIBTIME_H

#include <stdint.h>

/*! \brief Возвращает количество миллисекунд, прошедших со времени запуска
программы. Время переполнения этого счетчика (сброса обратно в нуль) около
50 дней.

\return 32-битное целое без знака, равное числу миллисекунд, прошедших с
момента подачи питания.
*/
uint32_t millis();

/*! \brief Возвращает количество микросекунд, прошедших со времени запуска
программы. Время переполнения этого счетчика (сброса обратно в нуль) около
70 минут.

\param 32-битное целое без знака, равное числу микросекунд, прошедших с
момента подачи питания.
*/
uint32_t micros();

#endif /* LABLIB_LIBTIME_H */