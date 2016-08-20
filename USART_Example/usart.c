#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "ringbuffer.h"

#ifdef USE_BUFFERED_IO
struct ringbuffer rx_buffer, tx_buffer;
FILE uart0_iostream = FDEV_SETUP_STREAM(usart0_putchar_buffered, usart0_getchar_blocking_buffered, _FDEV_SETUP_RW);
#else
FILE uart0_iostream = FDEV_SETUP_STREAM(usart0_putchar, usart0_getchar_blocking, _FDEV_SETUP_RW);
#endif

#define USART0_BUFFER_EMPTY       ( UCSR0A & (1 << UDRE0) )
#define USART0_RECIEVE_COMPLETE   ( UCSR0A & (1 << RXC0) )

/* инициализация USART0 */
void usart0_init(unsigned int ubrr) {
    /* Настройка асинхронного режима 9600-8-N-1 */
    UBRR0H=(unsigned char)(ubrr>>8);
    UBRR0L=(unsigned char) ubrr;
    UCSR0A=0x00;
    /* Разрешение работы передатчика и приемника */
    UCSR0B=(1<<RXEN0)|(1<<TXEN0);
    /* Установка формата посылки: 8 бит данных, 1 стоп-бит */
    UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
    
#ifdef USE_BUFFERED_IO
    UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0);
    ringbuffer_init(&rx_buffer);
    ringbuffer_init(&tx_buffer);
#endif
}

/* Функция вывода символов в USART0 */
int usart0_putchar(char c, FILE *stream) {
    /* Вывод символа */
    while (!USART0_BUFFER_EMPTY);
    UDR0 = c;
    return 0;
}

/* Прием символа, если таковые имеются */
char usart0_getchar_nonblocking(FILE *stream) {
    static char dat;
    dat = 0;
    /* Проверка на наличие новых данных */
    if (USART0_RECIEVE_COMPLETE) dat = UDR0;
    /* Функция вернет 0, если не было новых данных */
    return dat;
}

/* Прием символа, если таковые имеются */
char usart0_getchar_blocking(FILE *stream) {
    /* зависаем в цикле, пока не появятся новые данные */
    while (!USART0_RECIEVE_COMPLETE) ;
    return UDR0;
}

#ifdef USE_BUFFERED_IO
ISR(USART0_RX_vect) {
    /* check recieve flags here */
    char dat = UDR0;
    ringbuffer_insert(&rx_buffer, dat);
}

ISR(USART0_TX_vect) {
    if (!ringbuffer_is_empty(&tx_buffer)) {
        char dat = ringbuffer_remove(&tx_buffer);
        UDR0 = dat;
    }
}

char usart0_getchar_blocking_buffered(FILE *stream) {
    while (ringbuffer_is_empty(&rx_buffer)) ;
    return ringbuffer_remove(&rx_buffer);
}

/* Функция вывода символов в USART0 */
int usart0_putchar_buffered(char c, FILE *stream) {
    /* Вывод символа */
    if (USART0_BUFFER_EMPTY && ringbuffer_is_empty(&tx_buffer)) {
        UDR0 = c;
    }
    else {
        while (ringbuffer_is_full(&tx_buffer)) ;
        ringbuffer_insert(&tx_buffer, c);
    }
    return 0;
}
#endif