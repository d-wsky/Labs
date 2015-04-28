/*
 * Interrupts_Example.c
 *
 * Created: 03.10.2011 3:07:39
 *  Author: денис васильковский
 */ 

#include <avr/io.h>
/* заголовочный файл с описанием макроса ISR */
#include <avr/interrupt.h>

/* функция-обработчик прерывания по INT0 */
ISR(INT0_vect) {
	PORTA ^=1;
}

int main(void)
{
	volatile int a = 0x1234, b = 0x5678, c;
	/* настраиваем сигнальную ножку на вывод */
	DDRA = 0x01;	
	/* разрешаем прерывания по INT0 по переднему фронту */
	EICRA = (1 << ISC01) | (1 << ISC00);
	/* снимаем маску с прерывания */
	EIMSK = (1 << INT0);
	/* разрешаем прерывания глобально */
	sei();
	
    while(1)
    {
        //TODO:: Please write your application code 
		c = a + b;
    }
}