/*
 * Ports_Example.c
 *
 * Created: 05.09.2011 17:47:27
 *  Author: денис васильковский
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

int main(void)
{
	char i=1;
	
	DDRA = 0xFF;
	
    while(1)
    {
        //TODO:: Please write your application code 
		PORTA = i;
		
		i = i << 1;
		
		_delay_ms(1000);
    }
}