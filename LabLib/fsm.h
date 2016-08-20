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
 *   About: ������ ��� ��������� �������� �������� ��������� �� ����� ��.
 *          ������������ ������ ��������� ����������� �������� ������� � ����
 *          �������, ���������� ��������� (State) � ������� ������� (Event),
 *          ������� ����� ��������� �������� ������� � ���� ���������. ����� 
 *          ����, � ����� ������� ����������� ������� �������� (Guard), �
 *          ����� ������������ �������� (Action), ������� ����� ������� �
 *          ���������.
 *
 * Details: ��� �������� ������ ������ � �������� ������������ ����� ����� ��
 *          ��������, ��������� � ����� ��: �������� ������� (�� ����
 *          �� �������� ����) ���������, ������� ������ ���� ������������
 *          � ������� ��������� ��� ������. ��������� �� void �� ���
 *          ����������� ����������� ����������������� ������������ ������
 *          (��������, �������� int *, � �������� float *), ��, � ���������,
 *          ��� ������������ ����������� �� ����������� ����������� ��
 *          ������������ ������������� ������.
 */ 


#ifndef FSM_H
#define FSM_H

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*! \brief ��� ������, ������������ ��� ����������� ��������� ���������
��������.*/
typedef uint8_t FsmState_t;

/*! \brief ��������� ��� ������� ������� ��������. 

\details ������� ������ ���� ����� ������� ��� ������������� �������,
� ���� ��� ����� �������� ��������, �� ������� ��������� ������� ��������
�������-�������� (��. \ref FsmAction_t), � ����� ������� � ����� ���������.

����� ��� �������-��������, ��� ������� ����� ��������������� ���� �������,
������� ���� �������� � ������ ������������� ������� (��. \ref fsmPostEvent()).
��������� �� ��� ������ ������������ � ���� \a p_data.

\param data_ptr - ������, ������� ���� �������� ������ � ������������ �
�������, ������� � ������ ������ �������������� �������� ���������.
\return true - �������� ������� ��������� �������� � ����� ���������.
        false - ��������, ��� ��������� ������� �� ����� �����������, ��
		        ������� ����� ����������� �����.
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
