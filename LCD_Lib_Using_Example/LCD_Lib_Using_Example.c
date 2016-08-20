/*
 * LCD_Lib_Example.c
 *
 * Created: 05.04.2014 16:59:57
 *  Author: Denis Vasilkovsky
 *
 *   About: ������ ������������� ���������������� ���������� ���, �����
 *          ������ � ����������� ���������� �� ���. � ������ �������� ��������
 *          ��� lcd.a ������, �������� ������������ ���������� ��������������
 *          � ������ ������� �� �� ����������.
 */ 

#include <avr/io.h>
#include "Libs/lcd.h"

/* ������������� ������,
������������ � ��� */
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