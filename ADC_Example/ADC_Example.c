/*
 * ADC_Example.c
 *
 * Created: 26.08.2011 13:34:04
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример работы со встроенным АЦП контроллера. Функция read_adc 
 *          сообщает текущее напряжение на выбранной ножке adc_input, используя
 *          метод опроса флага завершения преобразования (polling). Полученный
 *          результат для жестко закодированного канала PF1 отображается в виде
 *          двоичных данных на светодиодах портов A и B.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

// Функция запуска преобразования АЦП и
// чтения результата
unsigned int read_adc(unsigned char adc_input) {
	// Сбрасываем прошлый канал
    ADMUX &=~((1 << MUX0) | (1 << MUX1) | (1 << MUX2));
	// И выбираем новый
    ADMUX |= adc_input;
    // Задержка на установление опорного напряжения
    _delay_us(10);
    // Запуск преобразования АЦП
	ADCSRA |= (1 << ADSC);
    // Ожидание окончания преобразования
    while ((ADCSRA & (1 << ADIF))==0);
	// Сброс флага прерывания
    ADCSRA |= (1 << ADIF);
    return ADC;
}

int main(void) {
	// Настраиваем PORTA и PORTB на вывод
	DDRA  = 0xFF;
	PORTA = 0x00;
	DDRB  = 0xFF;
	PORTB = 0x00;
		
	// Выбираем источник опорного напряжения АЦП
	ADMUX |= (1 << REFS0);
	// Выбираем предделитель и включаем АЦП в работу
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |
	          (1 << ADPS0);
		
    while(1) {
        // Считывание результата преобразования.
		int adc_result;
		adc_result = read_adc(PF1);
		// Вывод результата
		PORTB = adc_result;
		PORTA = adc_result >> 8;
    }
}