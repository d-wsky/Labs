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
 * utils.h
 *
 * Created: 18-Aug-16 01:21:08
 *  Author: Denis Vasilkovskii
 *
 *   About: Сборник вспомогательных алгоритмов, облегчающих частые операции.
 */ 


#ifndef UTILS_H_
#define UTILS_H_

/*! \brief Этот макрос вычисляет размер массива, если он известен на этапе
компиляции программы.

Если массив передаётся в качестве указателя, то ничего не получится.
Например:

\code
void printArrSize(uint8_t * arr_ptr) {
    printf("Размер массива arr_ptr = %d", ARRAY_SIZE(arr_ptr));
}

void loop() {
    uint8_t arr[] = {1, 2, 3, 4, 5};
    printArrSize(arr);

    printf("Размер массива arr = %d", ARRAY_SIZE(arr));
}
\endcode

Результат исполнения такой программы:

\code
// Размер массива arr_ptr = 0
// Размер массива arr = 5
\endcode
*/
#define ARRAY_SIZE(a)              (sizeof(a)/sizeof(a[0]))


#endif /* UTILS_H_ */