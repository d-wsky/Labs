/*
 * lcd.h
 *
 * Created: 04.05.2014 17:12:42
 *  Author: Денис Васильковский
 */ 


#ifndef LCD_H_
#define LCD_H_

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

/* функция передачи байта в жки. 
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
	 rs = 1 - данные (сбрасывается линия RS) */
void lcd_putbyte(char c, char rs);

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void lcd_clear();

/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
void lcd_gotoxy(char col, char row);

/* функция перемещения курсора в начальную
позицию (0, 0) */
void lcd_home();

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void lcd_init(LcdPhysicalParams_t ph, 
              LcdOutParams_t out,
			  LcdCursorMode_t cur);

#endif /* LCD_H_ */