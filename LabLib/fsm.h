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
 * fsm.h
 *
 * Created: 11-Aug-16 23:10:38
 *  Author: Denis Vasilkovskii
 *
 *   About: Модуль для упрощения описания конечных автоматов на языке Си.
 *          Предлагаемая модель позволяет представить конечный автомат в виде
 *          таблицы, содержащей состояния (State) и внешние события (Event),
 *          которые могут перевести конечный автомат в иное состояние. Кроме 
 *          того, в такой таблице указывается условие перехода (Guard), а
 *          также произвольное действие (Action), которое можно связать с
 *          переходом.
 *
 * Details: Для передачи данных вместе с событием используется самый общий из
 *          подходов, доступных в языке Си: передача пустого (то есть
 *          не имеющего типа) указателя, который должен быть преобразован
 *          в заранее известный тип данных. Указатель на void не даёт
 *          возможности компилятору проконтролировать человеческую ошибку
 *          (например, передали int *, а получили float *), но, к сожалению,
 *          это единственная возможность не накладывать ограничений на
 *          передаваемые пользователем данные.
 */ 


#ifndef FSM_H
#define FSM_H

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*! \brief Тип данных, используемый для обозначения состояния конечного
автомата.*/
typedef uint8_t FsmState_t;

/*! \brief Интерфейс для функции условия перехода. 

\details Функция такого типа будет вызвана при возникновении события,
и если она вернёт истинное значение, то переход конечного автомат исполнит
функцию-действие (см. \ref FsmAction_t), а затем перейдёт в новое состояние.

Также как функция-действие, эта функция может воспользоваться теми данными,
которые были переданы в момент возникновения события (см. \ref fsmPostEvent()).
Указатель на эти данные представлены в виде \a p_data.

\param data_ptr - Данные, которые были переданы вместе с уведомлением о
событии, которое в данный момент обрабатывается конечным автоматом.
\return true - означает переход конечного автомата в новое состояние.
        false - означает, что указанный переход не будет осуществлен, но
		        таблица будет проверяться далее.
*/
typedef bool (*FsmGuard_t)(void * p_data);


typedef void (*FsmAction_t)(void * p_data);
typedef uint8_t FsmEvent_t;

typedef  struct {
	FsmEvent_t event;
	FsmGuard_t guard;
	FsmAction_t action;
	FsmState_t next_state;
} FsmTableItem_t;

typedef FsmTableItem_t FsmTable_t;

#define FSM_NO_GUARD     NULL
#define FSM_NO_ACTION    NULL
#define FSM_NO_EVENT     0xFF
#define FSM_OTHERWISE    NULL
#define FSM_SAME_STATE   0xFE
#define FSM_ANY_STATE    0xFF

#define FSM_STATE(s)       \
	{ .event = FSM_NO_EVENT, .guard = NULL,  .action = NULL,   .next_state = (s) }

#define FSM_TRANSITION(Event, Guard, Action, NextState)     \
	{ .event = Event,        .guard = Guard, .action = Action, .next_state = NextState }

typedef struct {
	FsmState_t   current_state;
	FsmTable_t * table;
	uint8_t      table_size;
} Fsm_t;

void fsmInit(Fsm_t * fsm, FsmTable_t * table, uint8_t table_size, FsmState_t init_state);
void fsmEventPost(FsmEvent_t event, Fsm_t * fsm, void * p_data);
static inline FsmState_t fsmCurrentState(Fsm_t * fsm) {
	return fsm->current_state;
}

#endif /* FSM_H */
