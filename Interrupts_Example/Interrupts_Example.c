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
 * Interrupts_Example.c
 *
 * Created: 03.10.2011 3:07:39
 *  Author: Denis Vasilkovskii
 *
 *   About: ѕример программы, использующей прерывани€ на внешней ножке 
 *          PD0 (INT0), дл€ того, чтобы переключать состо€ние светодиода PA0.
 *           роме того, будучи скомпилированным и запущенным в режиме отладки,
 *          данный пример демонстрирует процесс сложени€ многобайтовых
 *          переменных, когда складывать можно только побайтово, а также
 *          механизм сохранени€-восстановлени€ контекста при вызове прерывани€.
 */ 

#include "structure.h"
#include "gpio.h"
#include "interrupts.h"

void irq0_handler(void) {
    GpioLevel_t level = gpioPinGet(GPIO_PORTD, GPIO_PIN0);
    if (level == GPIO_LEVEL_HIGH) {
        gpioPinSet(GPIO_PORTA, GPIO_PIN0);
    }
    else {
        gpioPinClear(GPIO_PORTA, GPIO_PIN0);
    }
}

void setup() {
    gpioPinModeSet(GPIO_PORTA, GPIO_PIN0, GPIO_MODE_OUT);
    gpioPinModeSet(GPIO_PORTD, GPIO_PIN0, GPIO_MODE_IN);
    interruptAttach(IRQ_PIN_0, irq0_handler, IRQ_MODE_CHANGE);
}

void loop() {
    volatile int a = 0x1234, b = 0x5678, c;
    c = a + b;
    // следующа€ строка нужна, чтобы избавитьс€ от ошибки
    // variable 'c' set but not used
    (void)c;
}
