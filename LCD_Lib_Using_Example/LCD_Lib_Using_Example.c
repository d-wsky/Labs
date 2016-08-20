/*
 * LCD_Lib_Example.c
 *
 * Created: 05.04.2014 16:59:57
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример использования скомпилированной библиотеки ЖКИ, чтобы
 *          успеха в отображении информации на ЖКИ. В данном варианте исходный
 *          код lcd.a закрыт, позволяя пользователю библиотеки сосредоточится
 *          в первую очередь на ее применении.
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