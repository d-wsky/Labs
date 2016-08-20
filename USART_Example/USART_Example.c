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
#include <avr/interrupt.h>
#include "hd44780.h"
#include "usart.h"

int main(void) {

    unsigned char dat=0;
    DDRC = 0xFF;
    
    usart0_init(UBRR_FROM_BAUD(UART0_BAUD));
    /* Привязываем основной поток вывода к u0stdout */
    stdout = &uart0_iostream;
    stdin = &uart0_iostream;
    /* Инициализация и очистка дисплея */
    hd44780_init(Phys2Row5x8, OutNorm, CursorMode1Blink);
    hd44780_clear();
    
    sei();
    
    /* Пример вывода текста с использованием функции printf */
    printf("\033[2JSimple example of using USART-RS232 bridge.\r\n\r\n");
    printf("All that you type appears on LCD.\r\n");
    printf("USART echoes all typed symbols back so they appears here...\r\n\r\n");
    
    while(1) {
        /* Проверяем приемный буфер */
        dat = getchar();
        /* Когда поступит новый символ, выводим его на ЖКИ и в USART */
        hd44780_putc(dat);
        putchar(dat);
    };
}
