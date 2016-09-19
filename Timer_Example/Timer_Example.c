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
 * Timer_Example.c
 *
 * Created: 13-Sep-16 00:37:48
 * Author : Denis Vasilkovskii
 *
 *  About : Самый простой пример использования библиотеки LibTime.h. Здесь
 *          ножка, определяемая переменной blink_pin переключается с частотой
 *          в 1 Гц основываясь на значении системного времени. Последнее
 *          определяется при обращении к функции millis().
 */ 

#include "gpio.h"
#include "libtime.h"
#include "structure.h"

Gpio_t blink_pin = {
    .port = GPIO_PORTA,
    .pin = GPIO_PIN0
};

void setup() {
    gpioPinModeSet(blink_pin, GPIO_MODE_OUT);
}

void loop() {
    if (millis() % 1000 == 0) {
        gpioPinClear(blink_pin);
    }
    else if (millis() % 500 == 0) {
        gpioPinSet(blink_pin);
    }
}
