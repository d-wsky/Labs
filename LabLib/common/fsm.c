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
 * fsm.c
 *
 * Created: 12-Aug-16 01:11:41
 *  Author: Denis Vasilkovskii
 */ 

#include "fsm.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Универсальное средство для того, чтобы сообщить, что функция не нашла то, что
искала.*/
#define NOT_FOUND          UINT8_MAX

/* Возвращает истину, если текущий элемент таблицы является описанием состояния.*/
static inline bool IsState(FsmTableItem_t * item) {
    return item->event == FSM_NO_EVENT;
}

/*! \brief Линейно проходит таблицу, описывающую конечный автомат и находит в нём
состояние. Следом за состоянием будут идти события, привязанные к этому
состоянию.

\param fsm Указатель на конечный автомат.
\param state Состояние, которое необходимо найти.
\param starting_pos Позиция в таблице, с которой начинается поиск.

\return Индекс в таблице или #NOT_FOUND, если такое состояние не было найдено.*/
static uint8_t FindState(Fsm_t * fsm, FsmState_t state, uint8_t starting_pos) {
    for (uint8_t i = starting_pos; i < fsm->table_size; i++) {
        if (IsState(&fsm->table[i])) {
            if (fsm->table[i].next_state == state) {
                return i;
            }
        }
    }
    return NOT_FOUND;
}

/*! \brief Линейно проходит таблицу, описывающую конечный автомат и находит в нём
заданное событие. Останавливает поиск, если оказывается, что текущий элемент
таблицы описывает состояние.

\param fsm Указатель на конечный автомат.
\param event Событие, которое необходимо найти.
\param starting_pos Позиция в таблице, с которой начинается поиск.

\return Индекс в таблице или #NOT_FOUND, если такое событие не было найдено.*/
static uint8_t FindEvent(Fsm_t * fsm, FsmEvent_t event, uint8_t starting_pos) {
    for (uint8_t i = starting_pos; i < fsm->table_size; i++) {
        if (fsm->table[i].event == event) {
            return i;
        }
        if (IsState(&fsm->table[i])) {
            break;
        }
    }
    return NOT_FOUND;
}

/*! \brief Пробует найти и исполнить событие для заданного состояния.

Алгоритм работает в два этапа. На первом этапе осуществляется поиск переданного
состояния в таблице. Если поиск был удачен, начинается поиск переданного события
в этом состоянии. Если и в этом случае есть попадание, то проверяется условие
(guard) и производится исполнение заданного действия (action), после чего
алгоритм завершает свою работу.

\param fsm Указатель на конечный автомат.
\param state Состояние, для которого предлагается исполнить событие.
\param event Событие, которое предлагается обработать конечному автомату.
\param p_data Приватные данные, которые были переданы вместе с событием.

\return Истина, если событие было успешно обработано.
        Ложь, если не пустило условие или событие не было найдено для данного
        состояния.*/
static bool TryStateForEvent(Fsm_t * fsm, FsmState_t state, FsmEvent_t event, void * p_data) {
    uint8_t state_position = FindState(fsm, state, 0);
    if (state_position == NOT_FOUND) {
        return false;
    }

    uint8_t event_position = state_position;
    // сложная семантика для предиката цикла - соединённое присвоение и проверка
    // сначала производится вызов FindEvent, потом результат вызова кладётся
    // в event_position и, наконец, производится сравнение значения event_position
    // с константой NOT_FOUND
    while ((event_position = FindEvent(fsm, event, event_position + 1)) != NOT_FOUND) {
        FsmTableItem_t * p_item = &fsm->table[event_position];
        bool guard_pass = true;
        if (p_item->guard != FSM_NO_GUARD) {
            guard_pass = p_item->guard(p_data);
        }
        if (guard_pass) {
            if (p_item->action != FSM_NO_ACTION) {
                p_item->action(p_data);
            }
            if (p_item->next_state != FSM_SAME_STATE) {
                fsm->current_state = p_item->next_state;
            }
            return true;
        }
    }

    return false;
}

void fsmInit(Fsm_t * fsm, FsmTable_t * table, uint8_t table_size, FsmState_t init_state) {
    fsm->table = table;
    fsm->current_state = init_state;
    fsm->table_size = table_size;
}

void fsmEventPost(FsmEvent_t event, Fsm_t * fsm, void * p_data) {
     if (!TryStateForEvent(fsm, fsm->current_state, event, p_data)) {
         TryStateForEvent(fsm, FSM_ANY_STATE, event, p_data);
     }
}
