﻿/* 
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
 * Created: 01-Oct-16 21:44:38
 *  Author: Denis Vasilkovskii
 *
 *   About: Это тест простого конечного автомата, который линейно перебирает
 *          свои состояния друг за другом.
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
    ST_THIRD,
} test_states_t;

typedef enum {
    EV_NEXT,
} test_events_t;

typedef enum {
    NOTHING_CALLED,
    FIRST_ACTION_CALLED,
    SECOND_ACTION_CALLED,
    THIRD_ACTION_CALLED,
} test_callback_states_t;

void act_first(void * p_data);
void act_second(void * p_data);
void act_third(void * p_data);

FsmTable_t test_table[] = {
    FSM_STATE     (ST_FIRST),
    FSM_TRANSITION(EV_NEXT, FSM_NO_GUARD, act_first,  ST_SECOND),

    FSM_STATE     (ST_SECOND),
    FSM_TRANSITION(EV_NEXT, FSM_NO_GUARD, act_second, ST_THIRD),

    FSM_STATE     (ST_THIRD),
    FSM_TRANSITION(EV_NEXT, FSM_NO_GUARD, act_third,  FSM_SAME_STATE),
};

void * private_data;
test_callback_states_t callback_state = NOTHING_CALLED;
uint32_t callback_counter = 0;
Fsm_t test_fsm;

void act_first(void * p_data)
{
    ASSERT_EQ(p_data, private_data);
    callback_state = FIRST_ACTION_CALLED;
    callback_counter++;
}

void act_second(void * p_data)
{
    ASSERT_EQ(p_data, private_data);
    callback_state = SECOND_ACTION_CALLED;
    callback_counter++;
}

void act_third(void * p_data)
{
    ASSERT_EQ(p_data, private_data);
    callback_state = THIRD_ACTION_CALLED;
    callback_counter++;
}

void setup() {
    private_data = (void *)0xDEADBABE;
    fsmInit(&test_fsm, test_table, ARRAY_SIZE(test_table), ST_FIRST);
}

void loop() {
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_FIRST);

    fsmEventPost(EV_NEXT, &test_fsm, private_data);
    ASSERT_EQ(callback_state,   FIRST_ACTION_CALLED);
    ASSERT_EQ(callback_counter, 1);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_SECOND);

    fsmEventPost(EV_NEXT, &test_fsm, private_data);
    ASSERT_EQ(callback_state,   SECOND_ACTION_CALLED);
    ASSERT_EQ(callback_counter, 2);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_THIRD);

    fsmEventPost(EV_NEXT, &test_fsm, private_data);
    ASSERT_EQ(callback_state,   THIRD_ACTION_CALLED);
    ASSERT_EQ(callback_counter, 3);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_THIRD);

    fsmEventPost(EV_NEXT, &test_fsm, private_data);
    ASSERT_EQ(callback_state,   THIRD_ACTION_CALLED);
    ASSERT_EQ(callback_counter, 4);
    ASSERT_EQ(fsmCurrentState(&test_fsm), ST_THIRD);

    _Exit(EXIT_OK);
}