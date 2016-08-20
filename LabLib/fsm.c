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

static inline bool IsState(FsmTableItem_t * item) {
	return item->event == FSM_NO_EVENT;
}

void fsmInit(Fsm_t * fsm, FsmTable_t * table, uint8_t table_size, FsmState_t init_state) {
	fsm->table = table;
	fsm->current_state = init_state;
	fsm->table_size = table_size;
}

static uint8_t FindState(Fsm_t * fsm, FsmState_t state, uint8_t starting_pos) {
	for (uint8_t i = starting_pos; i < fsm->table_size; i++) {
		if (IsState(&fsm->table[i])) {
			if (fsm->table[i].next_state == state) {
				return i;
			}
		}
	}
	return UINT8_MAX;
}

static uint8_t FindEvent(Fsm_t * fsm, FsmEvent_t event, uint8_t starting_pos) {
	for (uint8_t i = starting_pos; i < fsm->table_size; i++) {
		if (fsm->table[i].event == event) {
			return i;
		}
		if (IsState(&fsm->table[i])) {
			break;
		}
	}
	return UINT8_MAX;
}

static bool TryStateForEvent(Fsm_t * fsm, FsmState_t state, FsmEvent_t event, void * p_data) {
	uint8_t state_position = FindState(fsm, state, 0);
	if (state_position != UINT8_MAX) {
		uint8_t event_position = FindEvent(fsm, event, state_position + 1);
		while ((event_position != UINT8_MAX)) {
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
				return false;
			}
			event_position = FindEvent(fsm, event, event_position + 1);
		}
	}
	else {
		return false;
	}
	return true;
}

void fsmEventPost(FsmEvent_t event, Fsm_t * fsm, void * p_data) {
 	bool continue_with_any_state = TryStateForEvent(fsm, fsm->current_state, event, p_data);
 	if (continue_with_any_state) {
 		TryStateForEvent(fsm, FSM_ANY_STATE, event, p_data);
 	}
}
