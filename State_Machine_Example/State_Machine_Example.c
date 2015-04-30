/*
 * State_Machine_Example.c
 *
 * Created: 08.03.2014 22:39:27
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример бегущего вверх-вниз по порту A огонька, только реализованный
 *          в виде конечно автомата. Настоящий конечный автомат тут конечно не
 *          получается, поскольку еще не до конца известен синтаксис языка Си.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

/* Движение огонька вниз по отладочной плате, то есть в сторону к старшим разрядам */
#define ST_MOVING_UP    0
/* Движение огонька вверх по отладочной плате, то есть в сторону к младшим разрядам */
#define ST_MOVING_DOWN  1

int main(void)
{
	char i = 1, state = ST_MOVING_DOWN;
	
	DDRA = 0xFF;
	
    while(1)
    {
        // Обновление состояния порта
		PORTA = i;
		
		// Обновление конечного автомата
		switch (state) {
			case ST_MOVING_UP:
				// действие для состояния
				i = i >> 1;
				// проверка перехода
				if (i == 0x01)
					state = ST_MOVING_DOWN;
				break;
			case ST_MOVING_DOWN:
				// действие для состояния
				i = i << 1;
				// проверка перехода
				if (i == 0x80)
					state = ST_MOVING_UP;
				break;
		}
		
		// Задержка
		_delay_ms(1000);
    }
}