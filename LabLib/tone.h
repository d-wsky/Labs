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
 * tone.h
 *
 * Created: 01-Nov-16 22:47:07
 *  Author: Denis Vasilkovskii
 *
 *   About: Часть библиотеки продвинутого ввода вывода. Содержит описание
 *          функций, позволяющих выводить звуковой сигнал из микроконтроллера.
 */ 


#ifndef LABLIB_TONE_H
#define LABLIB_TONE_H

#include "gpio.h"
#include <stdint.h>

/*! \brief Если передать это значение в качестве аргумента функции tone(),
то звуковой сигнал будет продолжаться до тех пор, пока не вызовут функцию
noTone().*/
#define TONE_LENGTH_INFINITE     UINT32_MAX

/*! \brief Запускает генерацию меандра на указанной ножке.

\details После вызова этой функции, на ножке возникает периодический цифровой
сигнал со скважностью 50%. Кроме того можно задать длительность сигнала. Если
длительность установлена бесконечной (TONE_LENGTH_INFINITE), то для отключения
сигнала потребуется вызов функции noTone(). Если подключить указанную ножку
ввода-вывода к пьезо-динамику, то можно играть простые мелодии.

Нельзя воспроизвести два звуковых сигнала одновременно. Если функция tone()
уже была вызвана для другой ножки, то повторный вызов не делает ничего. Если
функцию tone() вызвать для той же самой ножки, то поменяется частота и
продолжительность сигнала.

\param pin - Ножка, на которой будет синтезирован сигнал.
\param frequency - Частота сигнала в Герцах (31-65535).
\param duration - Продолжительность сигнала в миллисекундах.

\return Ничего.
*/
void tone(Gpio_t pin, uint16_t frequency, uint32_t duration);

/*! \brief Прекращает генерацию звукового сигнала.

\details Не делает ничего, если функция tone() до этого не вызывалась.

\param pin - Ножка, на которой будет прекращен синтез сигнала.

\return Ничего.
*/
void noTone(Gpio_t pin);

#endif /* LABLIB_TONE_H */