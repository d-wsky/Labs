#pragma once

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

/* установка лог. "1" на линии E */
#define LCD_E_SET    PORTC|=0x02
/* установка лог. "0" на линии E */
#define LCD_E_CLR    PORTC&=0xFD   
/* установка лог. "1" на линии RS */
#define LCD_RS_SET   PORTC|=0x01   
/* установка лог. "0" на линии RS */
#define LCD_RS_CLR   PORTC&=0xFE   

/* макрос, указывающий функции, что
передаются команды */
#define LCD_COMMAND  0             
/* макрос, указывающий функции, что
передаются данные */
#define LCD_DATA     1             


#define lcd_putc(x)  lcd_putbyte(x, LCD_DATA)

/* функция передачи тетрады в жки */
extern void lcd_putnibble(char t);

/* функция передачи байта в жки. 
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
	 rs = 1 - данные (сбрасывается линия RS) */
extern void lcd_putbyte(char c, char rs);

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
extern void lcd_init();

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
extern void lcd_clear();

/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
extern void lcd_gotoxy(char col, char row);
