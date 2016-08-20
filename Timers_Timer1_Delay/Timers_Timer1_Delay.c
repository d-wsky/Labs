/*
 * Timers_Timer1_Delay.c
 *
 * Created: 29.03.2012 14:18:03
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример, демонстрирующий использование таймера-счетчика 1 для
 *          решения задачи, аналогичной Timers_Program_Delay. В этой программе
 *          для организации задержек используется ТС1, работающий в режиме CTC.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

// Частота МК
#define F_CPU           10000000UL
// Предделитель ТС1
#define TC_PRESCALER    1024
// Значения OCR1A соответствующие разным задержкам.
// Допустимые значения x = 1, 2, 3, 4, 5, 6
#define DELAY_VAL(x)    (x * (F_CPU / TC_PRESCALER))

// Задержка на дребезг контактов. Столько
// раз должно считаться значение лог. "1",
// чтобы считаться установившимся
#define DELAY_JITTER  250

// Функция-обработчик прерывания по
// совпадению с OCR1A
ISR(TIMER1_COMPA_vect) {
    // Переключение состояния светодиода
    PORTA = PORTA ^ 1;
}

// Функция считывает содержимое порта B,
// с защитой от дребезга - содержимое порта
// должно совпадать при считывании
// jit_delay раз подряд.
char button_state(char jit_delay) {
    char button, i;
    button = PINB;
        
    for (i = 0; i < jit_delay - 1; i++) {
        if (button != PINB) break;
    };
    
    return button;
}

// Функция, изменяющая интервал задержки, записывая
// новое значение в регистр сравнения OCR1A
char change_delay_time(char new_interval) {
    new_interval++;
    if (new_interval > 2) new_interval = 0;
    // Определение необходимого интервала
    switch (new_interval) {
        case 0: OCR1A = DELAY_VAL(1); break;
        case 1: OCR1A = DELAY_VAL(2); break;
        case 2: OCR1A = DELAY_VAL(4); break;
    };
    return new_interval;
}

// Предварительная настройка таймера
void timer_init() {
    // Инициализация Т/С1
    OCR1A  = DELAY_VAL(1);
    TCCR1A = 0x00;
    // Выбор режима CTC и предделителя
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1C = 0x00;
    // Разрешаем прерывание по достижению максимума
    TIMSK  = (1 << OCIE1A);
    ETIMSK = 0x00;
}

int main(void)
{
    // Инициализация переменных
    /* переключатель режимов задержки */
    char delay_interval = 0;
    /* защита от повторной обработки кнопки */
    char button_handled = 0;
    
    // Инициализация портов
    DDRA  = 0x01;
    DDRB  = 0x00;
    PORTB = 0x01;
    
    timer_init();
    
    // Глобальное разрешение прерываний
    sei();
    
    // Рабочий цикл
    while(1)
    {
        // Действия при нажатой кнопке
        if ((button_state(DELAY_JITTER)==1) && (!button_handled)) {
            // Изменяем переключатель задержки
            delay_interval = change_delay_time(delay_interval);
            // Переключение светодиода,
            // указывающее, что нажатие на 
            // кнопку было обработано
            TCNT1  = 0x0000;
            PORTA ^= 1;
            // Установка защелки
            button_handled = 1;
        };
        
        // Действия при отжатой кнопке
        if (button_state(DELAY_JITTER)==0)    
          // Снятие защелки
          button_handled = 0;
    }
}