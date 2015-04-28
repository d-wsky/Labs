/*
 * Traffic_Light_Example.c
 *
 * Created: 13-Jan-15 22:40:38
 *  Author: d.wsky
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

/* набор возможных состо€ний конечного автомата */
#define STATE_STOP                 1
#define STATE_READY_TO_MOVE        2
#define STATE_MOVE                 3
#define STATE_END_MOVEMENT         4
#define STATE_READY_TO_STOP        5
#define STATE_NO_TRAFFIC_CONTROL   6

/* значени€ задержек в секундах*/
#define MOVE_DELAY                 10
#define STOP_DELAY                 5

/* ¬спомогательна€ функци€ задержки, кратной секундам */
void delay_s(uint8_t t) {
	for (uint8_t i = 0; i < t; i++)
		_delay_ms(1000);	
}

/* —осто€ние запрещени€ движени€ в течение state_delay секунд.
√орит только красный индикатор. */
void Stop(uint8_t state_delay) {
	RED_LIGHT_ON;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_OFF;
	delay_s(state_delay);
}

/* —осто€ние подготовки к движению в течение 3 секунд.
√орит красный и желтый индикатор. */
void ReadyToMove() {
	RED_LIGHT_ON;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* —осто€ние движение разрешено в течение state_delay секунд.
√орит только зеленый индикатор */
void Move(uint8_t state_delay) {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_ON;
	delay_s(state_delay);
}

/* —осто€ние подготовки к остановке. ¬ течение 3 секунд
зеленый индикатор мигает с частотой 1 √ц. */
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

/* —осто€ние завершени€ движени€ в течение 3 секунд.
√орит только желтый индикатор. */
void ReadyToStop() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* —осто€ние отключени€ регулировки. ќдин раз
моргает желтым индикатором (ON->OFF), чтобы не блокироватьс€.*/
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
	/* инициализаци€ контроллера */
	DDRA = 0x07;
	/* инициализаци€ конечного автомата */
	uint8_t current_state = STATE_STOP;
	/* цикл работы конечного автомата */
    while(1) {
        switch (current_state) {
			case STATE_STOP:
				/* действие в этом состо€нии */
				Stop(STOP_DELAY);
				/* выбор следующего состо€ни€ */
				if (PINB & 1) {
					current_state = STATE_NO_TRAFFIC_CONTROL;
				}
				else {
					current_state = STATE_READY_TO_MOVE;
				}
				break;
			case STATE_READY_TO_MOVE:
				/* код следующего состо€ни€... */
				ReadyToMove();
				current_state = STATE_MOVE;
				break;
			case STATE_MOVE:
				/* код следующего состо€ни€... */
				Move(MOVE_DELAY);
				current_state = STATE_END_MOVEMENT;
				break;
			case STATE_END_MOVEMENT:
				/* код следующего состо€ни€... */
				EndMovement();
				current_state = STATE_READY_TO_STOP;
				break;
			case STATE_READY_TO_STOP:
				/* код следующего состо€ни€... */
				ReadyToStop();
				current_state = STATE_STOP;
				break;
			case STATE_NO_TRAFFIC_CONTROL:
				/* код следующего состо€ни€... */
				NoTrafficControl();
				if (!(PINB & 1)) {
					current_state = STATE_STOP;
				}
				break;
		}
    }
}