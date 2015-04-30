/*
 * LCD_4bit_WriteOnly.c
 *
 * Created: 28.07.2011 17:37:36
 *  Author: Денис Васильковский
 *
 *
 *  Edited: 19.03.2015 23:06:22
 *  Author: Денис Васильковский
 *  Reason: Добавлена поддержка OLED-контроллера WS0001
 *
 *   About: Самый простой пример использования ЖКИ (OLED) в качестве средства
 *          отображения символьной информации. В результате запуска и успешного
 *          исполнения программы, демонстрирует букву 'R' на третьей колонке
 *          второго ряда символов дисплея. Для корректной работы требует
 *          стандартного подключения ЖКИ к UNI-DS3.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

/* установка лог. "1" на линии E */
#define LCD_E_SET             ( PORTC |= 0x02 )
/* установка лог. "0" на линии E */
#define LCD_E_CLR             ( PORTC &= 0xFD )
/* установка лог. "1" на линии RS */
#define LCD_RS_SET            ( PORTC |= 0x01 )
/* установка лог. "0" на линии RS */
#define LCD_RS_CLR            ( PORTC &= 0xFE )
/* задержка на передачу данных в контроллер ЖКИ */
#define LCD_PUTNIBBLE_DELAY     _delay_us(50)

/* макрос, указывающий функции, что
передаются команды */
#define LCD_COMMAND             0             
/* макрос, указывающий функции, что
передаются данные */
#define LCD_DATA                1             

/* типы данных, используемые для инициализации дисплея */

/* физические параметры дисплея */
#define LCD_PHYSICAL_CMD        (1 << 5)
#define LCD_5x8MATRIX           (0 << 2)
#define LCD_5x10MATRIX          (1 << 2)
#define LCD_1ROW                (0 << 3)
#define LCD_2ROW                (1 << 3)
#define LCD_4BIT_BUS            (0 << 4)
#define LCD_8BIT_BUS            (1 << 4)
/* поддержка русской кодовой страницы WS0001 */
#define LCD_RUS_FONT_TABLE      (2 << 0)
typedef enum {
	Phys8Bit1Row5x8  = LCD_PHYSICAL_CMD | LCD_8BIT_BUS | LCD_1ROW | LCD_5x8MATRIX  | LCD_RUS_FONT_TABLE,
	Phys8Bit1Row5x10 = LCD_PHYSICAL_CMD | LCD_8BIT_BUS | LCD_1ROW | LCD_5x10MATRIX | LCD_RUS_FONT_TABLE,
	Phys8Bit2Row5x8  = LCD_PHYSICAL_CMD | LCD_8BIT_BUS | LCD_2ROW | LCD_5x8MATRIX  | LCD_RUS_FONT_TABLE,
	Phys8Bit2Row5x10 = LCD_PHYSICAL_CMD | LCD_8BIT_BUS | LCD_2ROW | LCD_5x10MATRIX | LCD_RUS_FONT_TABLE,
	Phys4Bit1Row5x8  = LCD_PHYSICAL_CMD | LCD_4BIT_BUS | LCD_1ROW | LCD_5x8MATRIX  | LCD_RUS_FONT_TABLE,
	Phys4Bit1Row5x10 = LCD_PHYSICAL_CMD | LCD_4BIT_BUS | LCD_1ROW | LCD_5x10MATRIX | LCD_RUS_FONT_TABLE,
	Phys4Bit2Row5x8  = LCD_PHYSICAL_CMD | LCD_4BIT_BUS | LCD_2ROW | LCD_5x8MATRIX  | LCD_RUS_FONT_TABLE,
	Phys4Bit2Row5x10 = LCD_PHYSICAL_CMD | LCD_4BIT_BUS | LCD_2ROW | LCD_5x10MATRIX | LCD_RUS_FONT_TABLE,
} LcdPhysicalParams_t;

/* направление вывода */
#define LCD_OUT_CMD             (1 << 2)
#define LCD_OUT_LEFT_TO_RIGHT   (1 << 1)
#define LCD_OUT_RIGHT_TO_LEFT   (0 << 1)
typedef enum {
	OutNorm   = LCD_OUT_CMD | LCD_OUT_LEFT_TO_RIGHT,
	OutArabic = LCD_OUT_CMD | LCD_OUT_RIGHT_TO_LEFT
} LcdOutParams_t;

/* варианты отображения курсора */
#define LCD_CURSOR_MODE_CMD     (1 << 3)
#define LCD_DISPLAY_ON          (1 << 2)
#define LCD_DISPLAY_OFF         (0 << 2)
#define LCD_CURSOR_ON           (1 << 1)
#define LCD_CURSOR_OFF          (0 << 1)
#define LCD_BLINK_ON            (1 << 0)
#define LCD_BLINK_OFF           (0 << 0)

typedef enum {
	DispOff          = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_OFF,
	CursorOff        = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF,
	CursorMode1      = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_ON | LCD_CURSOR_ON  | LCD_BLINK_OFF,
	CursorMode1Blink = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_ON | LCD_CURSOR_ON  | LCD_BLINK_ON,
	CursorMode2      = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF,
	CursorMode2Blink = LCD_CURSOR_MODE_CMD | LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_ON
} LcdCursorMode_t;

#define LCD_GOTO_CMD            (1 << 7)

#define lcd_putc(x)  lcd_putbyte(x, LCD_DATA)

/* инициализация портов,
подключенных к жки */
void init_port()
{
    PORTC = 0x00;
    DDRC  = 0xFF;
}

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

int main(void)
{
	init_port();
    lcd_init(Phys4Bit2Row5x8, OutNorm, CursorMode2Blink);
	lcd_clear();
	lcd_gotoxy(2, 1);
    lcd_putc('R');
    while (1) {
	};
	return 0;
}