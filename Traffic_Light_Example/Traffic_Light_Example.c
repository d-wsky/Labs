/*
 * Traffic_Light_Example.c
 *
 * Created: 13-Jan-15 22:40:38
 *  Author: d.wsky
 */ 


#include <avr/io.h>
#define F_CPU   10000000UL
#include <util/delay.h>

/* ���������� ������� ����������� */
#define RED_LIGHT_ON         PORTA |= (1 << PA0)
#define RED_LIGHT_OFF        PORTA &=~(1 << PA0)

/* ���������� ������� ����������� */
#define YELLOW_LIGHT_ON      PORTA |= (1 << PA1)
#define YELLOW_LIGHT_OFF     PORTA &=~(1 << PA1)

/* ���������� ������� ����������� */
#define GREEN_LIGHT_ON       PORTA |= (1 << PA2)
#define GREEN_LIGHT_OFF      PORTA &=~(1 << PA2)

/* ����� ��������� ��������� ��������� �������� */
#define STATE_STOP                 1
#define STATE_READY_TO_MOVE        2
#define STATE_MOVE                 3
#define STATE_END_MOVEMENT         4
#define STATE_READY_TO_STOP        5
#define STATE_NO_TRAFFIC_CONTROL   6

/* �������� �������� � ��������*/
#define MOVE_DELAY                 10
#define STOP_DELAY                 5

/* ��������������� ������� ��������, ������� �������� */
void delay_s(uint8_t t) {
	for (uint8_t i = 0; i < t; i++)
		_delay_ms(1000);	
}

/* ��������� ���������� �������� � ������� state_delay ������.
����� ������ ������� ���������. */
void Stop(uint8_t state_delay) {
	RED_LIGHT_ON;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_OFF;
	delay_s(state_delay);
}

/* ��������� ���������� � �������� � ������� 3 ������.
����� ������� � ������ ���������. */
void ReadyToMove() {
	RED_LIGHT_ON;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* ��������� �������� ��������� � ������� state_delay ������.
����� ������ ������� ��������� */
void Move(uint8_t state_delay) {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_OFF;
	GREEN_LIGHT_ON;
	delay_s(state_delay);
}

/* ��������� ���������� � ���������. � ������� 3 ������
������� ��������� ������ � �������� 1 ��. */
void EndMovement() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_OFF;
	/* ������� */
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

/* ��������� ���������� �������� � ������� 3 ������.
����� ������ ������ ���������. */
void ReadyToStop() {
	RED_LIGHT_OFF;
	YELLOW_LIGHT_ON;
	GREEN_LIGHT_OFF;
	delay_s(3);
}

/* ��������� ���������� �����������. ���� ���
������� ������ ����������� (ON->OFF), ����� �� �������������.*/
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
	/* ������������� ����������� */
	DDRA = 0x07;
	/* ������������� ��������� �������� */
	uint8_t current_state = STATE_STOP;
	/* ���� ������ ��������� �������� */
    while(1) {
        switch (current_state) {
			case STATE_STOP:
				/* �������� � ���� ��������� */
				Stop(STOP_DELAY);
				/* ����� ���������� ��������� */
				if (PINB & 1) {
					current_state = STATE_NO_TRAFFIC_CONTROL;
				}
				else {
					current_state = STATE_READY_TO_MOVE;
				}
				break;
			case STATE_READY_TO_MOVE:
				/* ��� ���������� ���������... */
				ReadyToMove();
				current_state = STATE_MOVE;
				break;
			case STATE_MOVE:
				/* ��� ���������� ���������... */
				Move(MOVE_DELAY);
				current_state = STATE_END_MOVEMENT;
				break;
			case STATE_END_MOVEMENT:
				/* ��� ���������� ���������... */
				EndMovement();
				current_state = STATE_READY_TO_STOP;
				break;
			case STATE_READY_TO_STOP:
				/* ��� ���������� ���������... */
				ReadyToStop();
				current_state = STATE_STOP;
				break;
			case STATE_NO_TRAFFIC_CONTROL:
				/* ��� ���������� ���������... */
				NoTrafficControl();
				if (!(PINB & 1)) {
					current_state = STATE_STOP;
				}
				break;
		}
    }
}