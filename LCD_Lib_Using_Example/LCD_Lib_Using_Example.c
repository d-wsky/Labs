/*
 * LCD_Lib_Example.c
 *
 * Created: 04.05.2014 16:59:57
 *  Author: Денис Васильковский
 */ 

#include <avr/io.h>
#include "Libs/lcd.h"

/* инициализация портов,
подключенных к жки */
void init_port() {
	PORTC=0x00;
	DDRC=0xFF;
}

int main(void)
{
	init_port();
	lcd_init();
	lcd_clear();
	lcd_gotoxy(2, 1);
	
	lcd_puts("Hello world!");
    while(1)
    {

    }
}