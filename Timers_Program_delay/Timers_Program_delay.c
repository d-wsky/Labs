﻿/*
 * Timers_Program_delay.c
 *
 * Created: 01.08.2011 16:59:19
 *  Author: денис васильковский
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

int main(void)
{
	// Инициализация переменных
	char delay_int = 0;
	char b;
	DDRA = 0x01;
	DDRB = 0x00;
	PORTB = 0x01;
	// Рабочий цикл
    while(1)
    {
		// Считывание значения на порте B
		b = PINB;
		if (b==1) delay_int++;
		if (delay_int>2) delay_int = 0;
		// Определение необходимого интервала
		// задержки и сама задержка
		switch (delay_int) {
			case 0: _delay_ms(1000); break;
			case 1: _delay_ms(2000); break;
			case 2: _delay_ms(4000); break;
        }
		// Переключение состояния светодиода
		PORTA ^= 1;
    }
}