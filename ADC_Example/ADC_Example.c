/*
 * ADC_Example.c
 *
 * Created: 26.08.2011 13:34:04
 *  Author: Denis Vasilkovsky
 *
 *   About: ������ ������ �� ���������� ��� �����������. ������� read_adc 
 *          �������� ������� ���������� �� ��������� ����� adc_input, ���������
 *          ����� ������ ����� ���������� �������������� (polling). ����������
 *          ��������� ��� ������ ��������������� ������ PF1 ������������ � ����
 *          �������� ������ �� ����������� ������ A � B.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

// ������� ������� �������������� ��� �
// ������ ����������
unsigned int read_adc(unsigned char adc_input) {
    // ���������� ������� �����
    ADMUX &=~((1 << MUX0) | (1 << MUX1) | (1 << MUX2));
    // � �������� �����
    ADMUX |= adc_input;
    // �������� �� ������������ �������� ����������
    _delay_us(10);
    // ������ �������������� ���
    ADCSRA |= (1 << ADSC);
    // �������� ��������� ��������������
    while ((ADCSRA & (1 << ADIF))==0);
    // ����� ����� ����������
    ADCSRA |= (1 << ADIF);
    return ADC;
}

int main(void) {
    // ����������� PORTA � PORTB �� �����
    DDRA  = 0xFF;
    PORTA = 0x00;
    DDRB  = 0xFF;
    PORTB = 0x00;
        
    // �������� �������� �������� ���������� ���
    ADMUX |= (1 << REFS0);
    // �������� ������������ � �������� ��� � ������
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |
              (1 << ADPS0);
        
    while(1) {
        // ���������� ���������� ��������������.
        int adc_result;
        adc_result = read_adc(PF1);
        // ����� ����������
        PORTB = adc_result;
        PORTA = adc_result >> 8;
    }
}