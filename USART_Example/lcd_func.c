#include "lcd_func.h"
#include <avr/io.h>
//#define F_CPU 10000000UL см. Project Properties -> Toolchain -> Symbols
#include <util/delay.h>

/* функция передачи тетрады в жки. передается
младшая тетрада входного байта */
void lcd_putnibble(char t)
{
    t <<= 4;
    LCD_E_SET;
    PORTC &= 0x0F;
    PORTC |= t;
    LCD_E_CLR;
    LCD_PUTNIBBLE_DELAY;
}

/* функция передачи байта в жки. 
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
	 rs = 1 - данные (сбрасывается линия RS) */
void lcd_putbyte(char c, char rs)
{
    char highc = 0;
    highc = c >> 4;
	if (rs == LCD_COMMAND) LCD_RS_CLR;
	else                   LCD_RS_SET;
    lcd_putnibble(highc);
    lcd_putnibble(c);
}

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void lcd_clear()
{
	lcd_putbyte(0x01, LCD_COMMAND);
	_delay_us(1500);
}

/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
void lcd_gotoxy(char col, char row)
{
	char adr;
	row &= 1;
	adr  = 0x40 * row + col;
	adr |= LCD_GOTO_CMD;
	lcd_putbyte(adr, LCD_COMMAND);
}

/* функция перемещения курсора в начальную
позицию (0, 0) */
void lcd_home()
{
	lcd_putbyte(0x02, LCD_COMMAND);
}

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void lcd_init(LcdPhysicalParams_t ph, 
              LcdOutParams_t out,
			  LcdCursorMode_t cur)
{
	_delay_ms(100);
	// Сброс шины данных
	LCD_RS_CLR; // установка нуля на линии RS
	lcd_putnibble(0x00);
	_delay_ms(5);
	lcd_putnibble(0x00);
	_delay_ms(5);
	lcd_putnibble(0x00);
	_delay_ms(5);
	lcd_putnibble(0x00);
	_delay_ms(5);
	lcd_putnibble(0x00);
	_delay_ms(5);
	
	// Выбор шины, дисплея
	lcd_putnibble(0x02);
	lcd_putnibble(0x02);
	lcd_putnibble(ph & 0x0f);
	// Здесь должна быть проверка флага занятости
	_delay_ms(10);
	
	// Display ON/OFF Control
	lcd_putbyte(CursorOff, LCD_COMMAND);
	// Здесь должна быть проверка флага занятости
	_delay_ms(10);
	
	// Очистка дисплея
	lcd_clear();
	// Здесь должна быть проверка флага занятости
	_delay_ms(10);

	// Режим вывода
	lcd_putbyte(out, LCD_COMMAND);
	
	// Дополнительно: режим работы курсора
	lcd_putbyte(cur, LCD_COMMAND);
	
	// Дополнительно: возврат в координату (0, 0)
	lcd_home();
}
