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
 * Created: 30-Oct-16 16:35:13
 *  Author: Denis Vasilkovskii
 *
 *   About: Это тест простого конечного автомата, в котором проверяется, что
 *          движок правильно отрабатывает несколько событий внутри одного
 *          состояния.
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
    EV_FIRST,
    EV_SECOND,
    EV_THIRD,
} test_events_t;

typedef enum {
    NOTHING_CALLED,
    FIRST_GUARD_CALLED,
    SECOND_GUARD_CALLED,
    THIRD_GUARD_CALLED,
    FIRST_ACTION_CALLED,
    SECOND_ACTION_CALLED,
    THIRD_ACTION_CALLED,
} test_callback_states_t;

void act_first(void * p_data);
void act_second(void * p_data);
void act_third(void * p_data);
bool guard_first(void * p_data);
bool guard_second(void * p_data);
bool guard_third(void * p_data);

FsmTable_t test_table[] = {
    FSM_STATE     (ST_FIRST),
    FSM_TRANSITION(EV_FIRST,  guard_first,     act_first,     ST_SECOND),
    FSM_TRANSITION(EV_SECOND, guard_second,    act_second,    ST_SECOND),

    FSM_STATE     (ST_SECOND),
    FSM_TRANSITION(EV_THIRD,  guard_third,     act_third,     ST_FIRST),
};

uint32_t callback_counter = 0;
test_callback_states_t callback_state = NOTHING_CALLED;
Fsm_t test_fsm;
const uint32_t THRESHOLD = 3;

bool guard_first(void * p_data) {
    callback_state = FIRST_GUARD_CALLED;
    FAIL("First guard should not be called ever");
    return true;
}

bool guard_second(void * p_data) {
    callback_state = SECOND_GUARD_CALLED;
    return true;
}

bool guard_third(void * p_data) {
    callback_state = THIRD_GUARD_CALLED;
    return true;
}

void act_first(void * p_data)
{
    callback_state = FIRST_ACTION_CALLED;
}

void act_second(void * p_data)
{
    ASSERT_EQ(callback_state, SECOND_GUARD_CALLED);
    callback_state = SECOND_ACTION_CALLED;
}

void act_third(void * p_data)
{
    ASSERT_EQ(callback_state, THIRD_GUARD_CALLED);
    callback_state = THIRD_ACTION_CALLED;
}

void setup() {
    fsmInit(&test_fsm, test_table, ARRAY_SIZE(test_table), ST_FIRST);
}

void loop() {
    fsmEventPost(EV_THIRD, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             NOTHING_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    fsmEventPost(EV_SECOND, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             SECOND_ACTION_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    callback_state = NOTHING_CALLED;

    fsmEventPost(EV_FIRST, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             NOTHING_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    fsmEventPost(EV_SECOND, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             NOTHING_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    fsmEventPost(EV_THIRD, &test_fsm, NULL);
    ASSERT_EQ(callback_state,             THIRD_ACTION_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    _Exit(EXIT_OK);
}
