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
 * fsm_linear.c
 *
 * Created: 28-Oct-16 01:00:34
 *  Author: Denis Vasilkovskii
 *
 *   About: Это тест простого конечного автомата, в котором отстутствует
 *          одно из действий, а также осуществляется переход назад по
 *          таблице описания.
 */ 

#include "fsm.h"
#include "structure.h"
#include "assert.h"
#include "utils.h"
#include "test_env.h"
#include <stdio.h>

typedef enum {
    ST_FIRST,
    ST_SECOND,
} test_states_t;

typedef enum {
    EV_NEXT,
} test_events_t;

typedef enum {
    NOTHING_CALLED,
    SECOND_ACTION_CALLED,
} test_callback_states_t;

void act_second(void * p_data);

FsmTable_t test_table[] = {
    FSM_STATE     (ST_FIRST),
    FSM_TRANSITION(EV_NEXT, FSM_NO_GUARD, FSM_NO_ACTION,  ST_SECOND),

    FSM_STATE     (ST_SECOND),
    FSM_TRANSITION(EV_NEXT, FSM_NO_GUARD, act_second,     ST_FIRST),
};

test_callback_states_t callback_state = NOTHING_CALLED;
uint32_t callback_counter = 0;
Fsm_t test_fsm;

void act_second(void * p_data)
{
    callback_state = SECOND_ACTION_CALLED;
    callback_counter++;
}

void setup() {
    fsmInit(&test_fsm, test_table, ARRAY_SIZE(test_table), ST_FIRST);
}

void loop() {
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    fsmEventPost(EV_NEXT, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             NOTHING_CALLED);
    ASSERT_EQ(callback_counter,           0);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    fsmEventPost(EV_NEXT, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             SECOND_ACTION_CALLED);
    ASSERT_EQ(callback_counter,           1);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    _Exit(EXIT_OK);
}
