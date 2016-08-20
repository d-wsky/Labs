/*
 * USART_Example.c
 *
 * Created: 27.02.2012 17:23:46
 *  Author: Denis Vasilkovsky
 *
 *   About: ������ ������������� ����������������� ���������� ��� ��������
 *          ������ ����� �� � ��. ��������� ���� UART0 �� ������� ���� ��
 *          ����������� 9600-8-N-1 � ����������� ������� ���������� �� ���.
 *          �� ����� �������� ���������� ������� �������, ����� ��� �����
 *          ������������ �� ���������.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "hd44780.h"
#include "usart.h"

int main(void) {

    unsigned char dat=0;
    DDRC = 0xFF;
    
    usart0_init(UBRR_FROM_BAUD(UART0_BAUD));
    /* ����������� �������� ����� ������ � u0stdout */
    stdout = &uart0_iostream;
    stdin = &uart0_iostream;
    /* ������������� � ������� ������� */
    hd44780_init(Phys2Row5x8, OutNorm, CursorMode1Blink);
    hd44780_clear();
    
    sei();
    
    /* ������ ������ ������ � �������������� ������� printf */
    printf("\033[2JSimple example of using USART-RS232 bridge.\r\n\r\n");
    printf("All that you type appears on LCD.\r\n");
    printf("USART echoes all typed symbols back so they appears here...\r\n\r\n");
    
    while(1) {
        /* ��������� �������� ����� */
        dat = getchar();
        /* ����� �������� ����� ������, ������� ��� �� ��� � � USART */
        hd44780_putc(dat);
        putchar(dat);
    };
}
