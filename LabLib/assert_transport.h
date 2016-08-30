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
 * assert_transport.h
 *
 * Created: 31-Aug-16 01:13:56
 *  Author: Denis Vasilkovskii
 *
 *   About: Этот файл описывает набор функций, которые позволяют определить
 *          свой собственный интерфейс по доставке сообщений о неправильном
 *          использовании библиотеки (Assert'ов).
 */ 


#ifndef LABLIB_ASSERT_TRANSPORT_H
#define LABLIB_ASSERT_TRANSPORT_H

/*! \brief Интерфейс функции, осуществляющей инициализацию транспортного
протокола передачи информации о случившемся Assert'е.*/
typedef void (*transport_init_t)();
/*! \brief Интерфейс функции, осуществляющей передачу информацию о самом
Assert'е в выбранный транспортный интерфейс.*/
typedef void (*transport_print_t)(const char * __func, const char * __file, 
                                  int __lineno, const char * __sexp);

/*! \brief Структура, описывающая транспортный интерфейс для передачи 
Assert'ов пользователю.*/
typedef struct {
    transport_init_t   init;
    transport_print_t  print;
} assert_transport_t;

/* список доступных на данный момент транспортов Assert'ов */
extern assert_transport_t lcd_assert_transport;

#endif /* LABLIB_ASSERT_TRANSPORT_H */