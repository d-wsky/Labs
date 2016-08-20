/*
 * Traffic_Light_Example.c
 *
 * Created: 13-Jan-15 22:40:38
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример более сложной задачи, решаемой через конечный автомат.
 *          Здесь на первых трех ножках порта PA0 симулируется стандартный
 *          светофор (PA0 - красный, PA1 - желтый, PA2 - зеленый). Светофор
 *          последовательно сменяет режимы, и в случае если в момент завершения
 *          отображения состояния STOP будет нажата кнопка PB0, он переходит
 *          в режим "нет управления трафиком".
 *          Подробно процесс решения данной задачи расписан в методических
 *          указаниях к работе №1.
 */ 


#include <avr/io.h>
#define F_CPU   10000000UL
#include <util/delay.h>

/* управление красным индикатором */
#define RED_LIGHT_ON         PORTA |= (1 << PA0)
#define RED_LIGHT_OFF        PORTA &=~(1 << PA0)

/* управление красным индикатором */
#define YELLOW_LIGHT_ON      PORTA |= (1 << PA1)
#define YELLOW_LIGHT_OFF     PORTA &=~(1 << PA1)

/* управление красным индикатором */
#define GREEN_LIGHT_ON       PORTA |= (1 << PA2)
#define GREEN_LIGHT_OFF      PORTA &=~(1 << PA2)

/* набор возможных состояний конечного автомата */
#define STATE_STOP                 1
#define STATE_READY_TO_MOVE        2
#define STATE_MOVE                 3
#define STATE_END_MOVEMENT         4
#define STATE_READY_TO_STOP        5
#define STATE_NO_TRAFFIC_CONTROL   6

/* значения задержек в секундах*/
#define MOVE_DELAY                 10
#define STOP_DELAY                 5

/* Вспомогательная функция задержки, кратной секундам */
void delay_s(uint8_t t) {
	for (uint8_t i = 0; i < t; i++)
		_delay_ms(1000);	
}

/* Состояние запрещения движения в течение state_delay секунд.
Горит только красный индикатор. */
void Stop(uint8_t state_delay) {
	RED_LIGHT_ON;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_OFF;
	delay_s(state_delay);
}

/* Состояние подготовки к движению в течение 3 секунд.
Горит красный и желтый индикатор. */
void ReadyToMove() {
	RED_LIGHT_ON;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* Состояние движение разрешено в течение state_delay секунд.
Горит только зеленый индикатор */
void Move(uint8_t state_delay) {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_ON;
	delay_s(state_delay);
}

/* Состояние подготовки к остановке. В течение 3 секунд
зеленый индикатор мигает с частотой 1 Гц. */
void EndMovement() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_OFF;
	/* мигание */
	for (uint8_t i = 0; i < 6; i++) {
		if (i & 1) {
			GREEN_LIGHT_OFF;
		}
		else {
			GREEN_LIGHT_ON;
		}
		_delay_ms(500);
	}
}

/* Состояние завершения движения в течение 3 секунд.
Горит только желтый индикатор. */
void ReadyToStop() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* Состояние отключения регулировки. Один раз
моргает желтым индикатором (ON->OFF), чтобы не блокироваться.*/
void NoTrafficControl() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(1);
	YELLOW_LIGHT_OFF;
	delay_s(1);
}

int main(void)
{
	/* инициализация контроллера */
	DDRA = 0x07;
	/* инициализация конечного автомата */
	uint8_t current_state = STATE_STOP;
	/* цикл работы конечного автомата */
    while(1) {
        switch (current_state) {
			case STATE_STOP:
				/* действие в этом состоянии */
				Stop(STOP_DELAY);
				/* выбор следующего состояния */
				if (PINB & 1) {
					current_state = STATE_NO_TRAFFIC_CONTROL;
				}
				else {
					current_state = STATE_READY_TO_MOVE;
				}
				break;
			case STATE_READY_TO_MOVE:
				/* код следующего состояния... */
				ReadyToMove();
				current_state = STATE_MOVE;
				break;
			case STATE_MOVE:
				/* код следующего состояния... */
				Move(MOVE_DELAY);
				current_state = STATE_END_MOVEMENT;
				break;
			case STATE_END_MOVEMENT:
				/* код следующего состояния... */
				EndMovement();
				current_state = STATE_READY_TO_STOP;
				break;
			case STATE_READY_TO_STOP:
				/* код следующего состояния... */
				ReadyToStop();
				current_state = STATE_STOP;
				break;
			case STATE_NO_TRAFFIC_CONTROL:
				/* код следующего состояния... */
				NoTrafficControl();
				if (!(PINB & 1)) {
					current_state = STATE_STOP;
				}
				break;
		}
    }
}