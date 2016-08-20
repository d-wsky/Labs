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
 * FSM_Example.c
 *
 * Created: 12-Aug-16 00:39:32
 * Author : Denis Vasilkovskii
 *
 *  About : Данный пример демонстрирует как можно пользоваться модулем fsm.h
 *          для описания и использования конечного автомата. Это делается на
 *          примере программы бегущего по кругу огонька: вниз по порту A,
 *          вправо до порта D, по нему наверх, влево до порта A и так далее.
 */ 

#include "structure.h"
#include "fsm.h"
#include "utils.h"
#include "gpio.h"
#include "delay.h"

typedef enum {
	S_MOVING_DOWN,
	S_MOVING_RIGHT,
	S_MOVING_UP,
	S_MOVING_LEFT,
} FsmStates_t;

typedef enum {
	E_TIMEOUT,
} FsmEvents_t;

typedef struct {
	GpioPort_t port;
	GpioPin_t  pin;
} LedPosition_t;

bool g_onBottom(void * p_data);
bool g_onRightSide(void * p_data);
bool g_onTop(void * p_data);
bool g_onLeftSide(void * p_data);
void a_moveDown(void * p_data);
void a_moveRight(void * p_data);
void a_moveUp(void * p_data);
void a_moveLeft(void * p_data);

Fsm_t fsm;
FsmTable_t fsmTable[] = {
 	FSM_STATE     ( S_MOVING_DOWN ),
 	FSM_TRANSITION( E_TIMEOUT, g_onBottom,    a_moveRight, S_MOVING_RIGHT ),
	FSM_TRANSITION( E_TIMEOUT, FSM_OTHERWISE, a_moveDown,  FSM_SAME_STATE ),

	FSM_STATE     ( S_MOVING_RIGHT ),
	FSM_TRANSITION( E_TIMEOUT, g_onRightSide, a_moveUp,    S_MOVING_UP ),
	FSM_TRANSITION( E_TIMEOUT, FSM_OTHERWISE, a_moveRight, FSM_SAME_STATE ),

	FSM_STATE     ( S_MOVING_UP ),
	FSM_TRANSITION( E_TIMEOUT, g_onTop,       a_moveLeft,  S_MOVING_LEFT ),
	FSM_TRANSITION( E_TIMEOUT, FSM_OTHERWISE, a_moveUp,    FSM_SAME_STATE ),

	FSM_STATE     ( S_MOVING_LEFT ),
	FSM_TRANSITION( E_TIMEOUT, g_onLeftSide,  a_moveDown,  S_MOVING_DOWN ),
	FSM_TRANSITION( E_TIMEOUT, FSM_OTHERWISE, a_moveLeft,  FSM_SAME_STATE ),
};

bool g_onBottom(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	return ledPosition->pin == GPIO_PIN7;
}

bool g_onRightSide(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	return ledPosition->port == GPIO_PORTD;
}

bool g_onTop(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	return ledPosition->pin == GPIO_PIN0;
}

bool g_onLeftSide(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	return ledPosition->port == GPIO_PORTA;
}

void a_moveDown(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	ledPosition->pin <<= 1;
}

void a_moveRight(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	ledPosition->port++;
}

void a_moveUp(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	ledPosition->pin >>= 1;
}

void a_moveLeft(void * p_data) {
	LedPosition_t * ledPosition = (LedPosition_t *)p_data;
	ledPosition->port--;
}

void updateLeds(const LedPosition_t * led) {
	gpioPinClear(GPIO_PORTA, GPIO_PIN_ALL);
	gpioPinClear(GPIO_PORTB, GPIO_PIN_ALL);
	gpioPinClear(GPIO_PORTC, GPIO_PIN_ALL);
	gpioPinClear(GPIO_PORTD, GPIO_PIN_ALL);
	gpioPinSet(led->port, led->pin);
}

void setup(void) {
	gpioPinModeSet(GPIO_PORTA, GPIO_PIN_ALL, GPIO_MODE_OUT);
	gpioPinModeSet(GPIO_PORTB, GPIO_PIN_ALL, GPIO_MODE_OUT);
	gpioPinModeSet(GPIO_PORTC, GPIO_PIN_ALL, GPIO_MODE_OUT);
	gpioPinModeSet(GPIO_PORTD, GPIO_PIN_ALL, GPIO_MODE_OUT);
	fsmInit(&fsm, fsmTable, ARRAY_SIZE(fsmTable), S_MOVING_DOWN);
}

void loop(void) {
	static LedPosition_t ledPosition = {
		.port = GPIO_PORTA,
		.pin  = GPIO_PIN0,
	};

	fsmEventPost(E_TIMEOUT, &fsm, &ledPosition);
	updateLeds(&ledPosition);
	delayMs(500);
}
