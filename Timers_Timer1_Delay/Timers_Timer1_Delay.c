/*
 * Timers_Timer1_Delay.c
 *
 * Created: 29.03.2012 14:18:03
 *  Author: Denis Vasilkovsky
 *
 *   About: ������, ��������������� ������������� �������-�������� 1 ���
 *          ������� ������, ����������� Timers_Program_Delay. � ���� ���������
 *          ��� ����������� �������� ������������ ��1, ���������� � ������ CTC.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// ������� ��
#define F_CPU           10000000UL
// ������������ ��1
#define TC_PRESCALER    1024
// �������� OCR1A ��������������� ������ ���������.
// ���������� �������� x = 1, 2, 3, 4, 5, 6
#define DELAY_VAL(x)    (x * (F_CPU / TC_PRESCALER))

// �������� �� ������� ���������. �������
// ��� ������ ��������� �������� ���. "1",
// ����� ��������� ��������������
#define DELAY_JITTER  250

// �������-���������� ���������� ��
// ���������� � OCR1A
ISR(TIMER1_COMPA_vect) {
    // ������������ ��������� ����������
    PORTA = PORTA ^ 1;
}

// ������� ��������� ���������� ����� B,
// � ������� �� �������� - ���������� �����
// ������ ��������� ��� ����������
// jit_delay ��� ������.
char button_state(char jit_delay) {
    char button, i;
    button = PINB;
        
    for (i = 0; i < jit_delay - 1; i++) {
        if (button != PINB) break;
    };
    
    return button;
}

// �������, ���������� �������� ��������, ���������
// ����� �������� � ������� ��������� OCR1A
char change_delay_time(char new_interval) {
    new_interval++;
    if (new_interval > 2) new_interval = 0;
    // ����������� ������������ ���������
    switch (new_interval) {
        case 0: OCR1A = DELAY_VAL(1); break;
        case 1: OCR1A = DELAY_VAL(2); break;
        case 2: OCR1A = DELAY_VAL(4); break;
    };
    return new_interval;
}

// ��������������� ��������� �������
void timer_init() {
    // ������������� �/�1
    OCR1A  = DELAY_VAL(1);
    TCCR1A = 0x00;
    // ����� ������ CTC � ������������
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1C = 0x00;
    // ��������� ���������� �� ���������� ���������
    TIMSK  = (1 << OCIE1A);
    ETIMSK = 0x00;
}

int main(void)
{
    // ������������� ����������
    /* ������������� ������� �������� */
    char delay_interval = 0;
    /* ������ �� ��������� ��������� ������ */
    char button_handled = 0;
    
    // ������������� ������
    DDRA  = 0x01;
    DDRB  = 0x00;
    PORTB = 0x01;
    
    timer_init();
    
    // ���������� ���������� ����������
    sei();
    
    // ������� ����
    while(1)
    {
        // �������� ��� ������� ������
        if ((button_state(DELAY_JITTER)==1) && (!button_handled)) {
            // �������� ������������� ��������
            delay_interval = change_delay_time(delay_interval);
            // ������������ ����������,
            // �����������, ��� ������� �� 
            // ������ ���� ����������
            TCNT1  = 0x0000;
            PORTA ^= 1;
            // ��������� �������
            button_handled = 1;
        };
        
        // �������� ��� ������� ������
        if (button_state(DELAY_JITTER)==0)    
          // ������ �������
          button_handled = 0;
    }
}