/*
 * USART_Example.c
 *
 * Created: 27.02.2012 17:23:46
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример использования последовательного интерфейся для передачи
 *          данных между ПК и МК. Используя порт UART0 МК слушает эфир на
 *          стандартных 9600-8-N-1 и принимаемые символы отображает на ЖКИ.
 *          Он также отсылает полученные символы обратно, чтобы они могли
 *          отобразиться на терминале.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include "lcd_func.h"
#include <stdio.h>

/* Макрос, указывающий текущую скорость обмена в бит/с */
#define BAUD 9600UL
/* Расчет значения, которое надо положить в регистр UBRR
для обеспечения связи на скорости BAUD при частоте МК F_CPU (Гц) */
#define UBRR_VAL  F_CPU/16UL/BAUD-1UL

/* Прототип функции-передатчика данных в поток */
static int usart0_putchar(char c, FILE *stream);

/* Описываем поток вывода через USART0 */
static FILE u0stdout = FDEV_SETUP_STREAM(usart0_putchar, NULL,
                                            _FDEV_SETUP_WRITE);

/* Функция вывода символов в USART0 */
static int usart0_putchar(char c, FILE *stream) {
	/* Дополняем символ '\n' символом '\r' */
    if (c == '\n')
    usart0_putchar('\r', stream);
	/* Вывод символа */
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = c;
    return 0;
}

/* инициализация USART0 */
void usart0_init(unsigned int ubrr) {
	// USART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud Rate: 9600
	UBRR0H=(unsigned char)(ubrr>>8);
	UBRR0L=(unsigned char) ubrr;
	UCSR0A=0x00;
	/* Разрешение работы передатчика и приемника */
	UCSR0B=(1<<RXEN0)|(1<<TXEN0);
	/* Установка формата посылки: 8 бит данных, 1 стоп-бит */
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
}

/* Прием символа, если таковые имеются */
unsigned char usart0_getchar(void) {
	static unsigned char dat;
	dat = 0;
	/* Проверка на наличие новых данных */
	if (UCSR0A & (1<<RXC)) dat = UDR0;
	/* Функция вернет 0, если не было новых данных */
	return dat;
}

int main(void) {

	DDRC = 0xFF;
	unsigned char dat=0;
	
	usart0_init(UBRR_VAL);
	/* Привязываем основной поток вывода к u0stdout */
	stdout = &u0stdout;
	/* Инициализация и очистка дисплея */
	lcd_init();
	lcd_clear();
	
	/* Пример вывода текста с использованием функции printf */
	printf("\033[2JSimple example of using USART-RS232 bridge.\n\n");
	printf("All that you type appears on LCD.\n");
	printf("USART echoes all typed symbols back so they appears here...\n\n");
	
	while(1) {
    	/* Проверяем приемный буфер */
		dat = usart0_getchar();
		/* Если пришел новый символ, то выводим его на ЖКИ и в
		USART */
		if (dat) {
			lcd_putc(dat);
			putchar(dat);
		};		
	};
}
