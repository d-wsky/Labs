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
 * Created: 28-Oct-16 01:03:06
 *  Author: Denis Vasilkovskii
 *
 *   About: Это тест простого конечного автомата, в котором несколько
 *          попыток перехода блокируются с помощью контрольной функции,
 *          а также осуществляется выбор действия в зависимости от значения
 *          данной функции.
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
    GUARD_CALLED,
    FIRST_ACTION_CALLED,
    SECOND_ACTION_CALLED,
    THIRD_ACTION_CALLED,
} test_callback_states_t;

void act_first(void * p_data);
void act_second(void * p_data);
void act_third(void * p_data);
bool guard_threshold(void * p_data);

FsmTable_t test_table[] = {
    FSM_STATE     (ST_FIRST),
    FSM_TRANSITION(EV_NEXT, guard_threshold, act_first,      ST_SECOND),

    FSM_STATE     (ST_SECOND),
    FSM_TRANSITION(EV_NEXT, guard_threshold, act_second,     ST_FIRST),
    FSM_TRANSITION(EV_NEXT, FSM_OTHERWISE,   act_third,      FSM_SAME_STATE),
};

uint32_t callback_counter = 0;
test_callback_states_t callback_state = NOTHING_CALLED;
Fsm_t test_fsm;
const uint32_t THRESHOLD = 3;

bool guard_threshold(void * p_data) {
    callback_counter++;
    uint32_t * p_int = (uint32_t *)p_data;
    callback_state = GUARD_CALLED;
    return *p_int == THRESHOLD;
}

void act_first(void * p_data)
{
    ASSERT_EQ(*(uint32_t *)p_data, THRESHOLD);
    callback_state = FIRST_ACTION_CALLED;
}

void act_second(void * p_data)
{
    callback_state = SECOND_ACTION_CALLED;
}

void act_third(void * p_data)
{
    ASSERT_EQ(callback_state, GUARD_CALLED);
    callback_state = THIRD_ACTION_CALLED;
}

void setup() {
    fsmInit(&test_fsm, test_table, ARRAY_SIZE(test_table), ST_FIRST);
}

void loop() {
    uint32_t threshold_check = 0;
    for (; threshold_check < THRESHOLD; threshold_check++) {
        fsmEventPost(EV_NEXT, &test_fsm, &threshold_check);
        ASSERT_EQ(callback_state,             GUARD_CALLED);
        ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);
        ASSERT_EQ(callback_counter,           threshold_check + 1);
    }

    fsmEventPost(EV_NEXT, &test_fsm, &threshold_check);
    ASSERT_EQ(callback_state,             FIRST_ACTION_CALLED);
    ASSERT_EQ(callback_counter,           threshold_check + 1);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    threshold_check = THRESHOLD + 1;
    fsmEventPost(EV_NEXT, &test_fsm, &threshold_check);
    ASSERT_EQ(callback_state,             THIRD_ACTION_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    threshold_check = THRESHOLD;
    fsmEventPost(EV_NEXT, &test_fsm, &threshold_check);
    ASSERT_EQ(callback_state,             SECOND_ACTION_CALLED);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    _Exit(EXIT_OK);
}
