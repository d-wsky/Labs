/*
 * lcd.h
 *
 * Created: 04.05.2014 17:12:42
 *  Author: Денис Васильковский
 */ 


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>

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

/* функция передачи байта в жки.*/
void lcd_putbyte(char c, char rs);

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void lcd_init();

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void lcd_clear();

/* функция перемещения курсора в заданную позицию.*/
void lcd_gotoxy(char col, char row);

/* вывод строки, до тех пор, пока не появится символ \0 */
void lcd_puts(char* s);

#endif /* LCD_H_ */