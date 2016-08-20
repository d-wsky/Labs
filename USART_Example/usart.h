#include <stdio.h>

/* ������, ����������� ������� �������� ������ � ���/� */
#define UART0_BAUD 9600UL
/* ������ ��������, ������� ���� �������� � ������� UBRR
��� ����������� ����� �� �������� BAUD ��� ������� �� F_CPU (��) */
#define UBRR_FROM_BAUD(x)  F_CPU/16UL/x-1UL

/* ����������������, ���� �� ����� �������������� ����-����� */
#define USE_BUFFERED_IO

/* ������������� USART0 */
void usart0_init(unsigned int ubrr) ;
/* ������� ������ �������� � USART0 */
int usart0_putchar(char c, FILE *stream) ;
/* ������� ������ �������� � USART0 */
int usart0_putchar_buffered(char c, FILE *stream) ;
/* ����� �������. ��������� 0, ���� ����� �������� � ���������� ������ ��� */
char usart0_getchar_nonblocking(FILE *stream) ;
/* ����� �������. �� �������� ����������, ���� �� ������� ������ */
char usart0_getchar_blocking(FILE *stream) ;

#ifdef USE_BUFFERED_IO
char usart0_getchar_blocking_buffered(FILE *stream);
int usart0_putchar_buffered(char c, FILE *stream) ;
#endif

/* ��������� ����� ������ ����� USART0 */
extern FILE uart0_iostream;

