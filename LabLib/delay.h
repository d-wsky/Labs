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
 * delay.h
 *
 * Created: 10-Aug-16 00:18:34
 *  Author: Denis Vasilkovskii
 *
 *   About: Функции синхронного ожидания.
 *
 * Details: Данные функции являются обёртками для библиотечных функций avr-gcc.
 *          Использованная обёртка добавляет небольшую дополнительную задержку,
 *          зато избавляет от одного из недостатков встроенных функций -
 *          необходимости знать значение задержки на этапе компиляции.
 *          Необходимый для их жизни макрос F_CPU спрятан в настройках
 *          компиляции библиотеки.
 */ 


#ifndef LABLIB_DELAY_H
#define LABLIB_DELAY_H

#include <stdint.h>

/* \brief Останавливает исполнение программы на заданное количество
миллисекунд.

\param ms - Количество времени, которое требуется подождать.

\todo Пример использования.

\details Важное замечание.

Использование функций задержки даёт простой способ реализовать взаимодействие
с некоторой периферией во времени, например, алгоритм антидребезга. Но как и
с любым простым решением, оно содержит в себе серьёзный встроенный недостаток.
Он состоит в том, что во время выполнения задержки, ни одно другое действие
выполняться не может (кроме прерываний), что может стать значительным
препятствием при разработке более или менее сложных программ.

Для преодоления этого недостатка, следует воспользоваться таймерами (см.
соответствующий раздел), а в особо запущенных случаях - вытесняющим
планировщиком (см. пример для операционной системы реального времени FreeRTOS).
*/
void delayMs(uint16_t ms);

/*! \brief Приостанавливает исполнение программы на заданное количество 
микросекунд.

\param us: количество микросекунд, которое требуется подождать.

\details Смотри раздел деталей для \ref delayMs().
*/
void delayUs(uint16_t us);

#endif /* LABLIB_DELAY_H */
