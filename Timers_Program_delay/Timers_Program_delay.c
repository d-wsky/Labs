/*
 * Timers_Program_delay.c
 *
 * Created: 01.08.2011 16:59:19
 *  Author: Denis Vasilkovsky
 *
 *   About: ������, ��������������� ���������� ������� � ����������� ��������
 *          ����������� �������� ����� ����� ������� _delay_ms(). ����������
 *          ���������: PA0 ������������� � ������ � 1 �. �������� ����� �����
 *          �������� �� 2 � � 4 �, ���� ������ �� ������ PINB0.
 *
 *    TODO: ��������� ��� ���������� ������.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

int main(void)
{
    // ������������� ����������
    char delay_int = 0;
    char b;
    DDRA = 0x01;
    DDRB = 0x00;
    PORTB = 0x01;
    // ������� ����
    while(1)
    {
        // ���������� �������� �� ����� B
        b = PINB;
        if (b==1) delay_int++;
        if (delay_int>2) delay_int = 0;
        // ����������� ������������ ���������
        // �������� � ���� ��������
        switch (delay_int) {
            case 0: _delay_ms(1000); break;
            case 1: _delay_ms(2000); break;
            case 2: _delay_ms(4000); break;
        }
        // ������������ ��������� ����������
        PORTA ^= 1;
    }
}