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
 * LCD_Example.c
 *
 * Created: 23-Aug-16 00:32:03
 * Author : Denis Vasilkovskii
 *
 *  About : Самый простой пример использования ЖКИ (OLED) в качестве средства
 *          отображения символьной информации. В результате запуска и успешного
 *          исполнения программы, демонстрирует текст "Hello world!" на экране
 *          дисплея. Для корректной работы требует стандартного подключения ЖКИ
 *          к UNI-DS3.
 */ 

#include "structure.h"
#include "lcd.h"
#include "gpio.h"

const lcdPinout_t lcdPinout = {
    .rs = {GPIO_PORTC, GPIO_PIN0},
    .rw = LCD_UNUSED_PIN,
    .e  = {GPIO_PORTC, GPIO_PIN1},
    .d0 = LCD_UNUSED_PIN,
    .d1 = LCD_UNUSED_PIN,
    .d2 = LCD_UNUSED_PIN,
    .d3 = LCD_UNUSED_PIN,
    .d4 = {GPIO_PORTC, GPIO_PIN4},
    .d5 = {GPIO_PORTC, GPIO_PIN5},
    .d6 = {GPIO_PORTC, GPIO_PIN6},
    .d7 = {GPIO_PORTC, GPIO_PIN7},
};

void setup() {
    lcdInit(&lcdPinout);
    lcdBegin(16, 2);
    lcdPrint("Hello world!");
}

void loop() {
    // здесь нарочно нет кода
}