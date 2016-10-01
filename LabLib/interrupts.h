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
 * interrupts.h
 *
 * Created: 10-Aug-16 00:45:03
 *  Author: Denis Vasilkovskii
 *
 *   About: Интерфейс для работы с прерываниями.
 */ 


#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/*! \brief Выбор ножки для слежения.*/
typedef enum {
    IRQ_PIN_0,          /*!< Ножка INT0 (PD0).*/
    IRQ_PIN_1,          /*!< Ножка INT1 (PD1).*/
    IRQ_PIN_2,          /*!< Ножка INT2 (PD2).*/
    IRQ_PIN_3,          /*!< Ножка INT3 (PD3).*/
    IRQ_PIN_4,          /*!< Ножка INT4 (PD4).*/
    IRQ_PIN_5,          /*!< Ножка INT5 (PD5).*/
    IRQ_PIN_6,          /*!< Ножка INT6 (PD6).*/
    IRQ_PIN_7,          /*!< Ножка INT7 (PD7).*/
    IRQ_SOURCES_AMOUNT, /*!< Это значение предназначено для внутреннего использования
                             внутри библиотеки.*/
} IrqPin_t;

/*! \brief Прототип пользовательской функции-обработчика прерываний.*/
typedef void (*Isr_t)(void);

/*! \brief Выбор типа события, на которое необходима реакция.*/
typedef enum {
    IRQ_MODE_LOW     = 0, /*!< Прерывание возникает всё время, пока на ножке
                               НИЗКОЕ значение напряжения.*/
    IRQ_MODE_CHANGE  = 1, /*!< Прерывание возникает при любом ИЗМЕНЕНИИ
                               логического уровня на ножке.*/
    IRQ_MODE_FALLING = 2, /*!< Прерывание возникает при ЗАДНЕМ (НИСПАДАЮЩЕМ)
                               фронте сигнала на ножке.*/
    IRQ_MODE_RISING  = 3, /*!< Прерывание возникает на ПЕРЕДНЕМ (ВОСХОДЯЩЕМ)
                               фронте сигнала на ножке.*/
} IrqMode_t;

/*! \brief Вызов функции осуществляет подключение к одному из источников
прерываний. 

\details Для того, чтобы подключиться к некоторому прерыванию, потребуется
подготовить три входных параметра для этой функции. Ножку (параметр 1) и
тип события (параметр 3) можно выбрать с помощью переключателей \ref IrqPin_t
и \ref IrqMode_t соответственно. В качестве второго параметра необходимо
указать функцию, которая должна: не иметь входных параметров и не возвращать
ничего. Другими словами, функцию вида:

\code
void interrupt_handler(void);
\endcode

Библиотека гарантирует, что данная функция будет вызвана в любой момент,
когда внутренние флаги микроконтроллера проинформируют об этом. Следует
помнить, что обработчик прерывания, вроде \a interrupt_handler выше
будет вызван в контексте прерывания, то есть заблокирует возможность
возникновения ЛЮБЫХ других событий в системе. Поэтому не рекомендуется
слишком сильно раздувать код, который будет находиться внутри данной функции.

Также важно все переменные, используемые внутри обработчика прерываний, 
объявлять с ключевым словом volatile, поскольку в этом случае оптимизатор
компилятора не станет исключать их из программы.

\param pin: Ножка, которая будет реагировать на уровень или изменение 
приложенного к ней напряжения.
\param isr:    Функция обработчик прерывания, которая должна присутствовать в
пользовательской программе. Эта функция должна не иметь входных параметров
и должна возвращать void.
\param mode: Определяет тип события, на который требуется реакция. См.
\ref IrqMode_t.
*/
void interruptAttach(IrqPin_t pin, Isr_t isr, IrqMode_t mode);

/*! \brief Отключение обработчика прерываний от ножки, а так же всякой
реакции системы на напряжение на указанной ножке.

\param Ножка, прерывания от которой следует выключить.
*/
void interruptDeattach(IrqPin_t pin);

/*! \brief Восстанавливает возможность возникновения прерываний, если они были
заблокированы с помощью \ref interruptsDisable().*/
void interruptsEnable();

/*! \brief Запрещает возникновение любых прерываний до тех пор, пока не
будет вызвана функция \ref interruptsEnable().*/
void interruptsDisable();

#endif /* INTERRUPTS_H */