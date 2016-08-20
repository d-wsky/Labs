#include <stdio.h>

/* Макрос, указывающий текущую скорость обмена в бит/с */
#define UART0_BAUD 9600UL
/* Расчет значения, которое надо положить в регистр UBRR
для обеспечения связи на скорости BAUD при частоте МК F_CPU (Гц) */
#define UBRR_FROM_BAUD(x)  F_CPU/16UL/x-1UL

/* Закомментировать, если не нужен буферизованный ввод-вывод */
#define USE_BUFFERED_IO

/* инициализация USART0 */
void usart0_init(unsigned int ubrr) ;
/* Функция вывода символов в USART0 */
int usart0_putchar(char c, FILE *stream) ;
/* Функция вывода символов в USART0 */
int usart0_putchar_buffered(char c, FILE *stream) ;
/* Прием символа. Возвратит 0, если новых символов в аппаратном буфере нет */
char usart0_getchar_nonblocking(FILE *stream) ;
/* Прием символа. Не завершит исполнения, пока не получит символ */
char usart0_getchar_blocking(FILE *stream) ;

#ifdef USE_BUFFERED_IO
char usart0_getchar_blocking_buffered(FILE *stream);
int usart0_putchar_buffered(char c, FILE *stream) ;
#endif

/* Описываем поток вывода через USART0 */
extern FILE uart0_iostream;

