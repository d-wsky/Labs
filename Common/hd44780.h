/*
 * hd44780.h
 *
 * Created: 11-Aug-15 23:38:56
 *  Author: Денис Васильковский
 *
 *   About: Интерфейсный файл для библиотеки ЖКИ контроллера HD44780 и
 *          совместимого с ним OLED контроллера Winstar.
 *
 *    Todo: Не поддерживается режим чтения данных из дисплея.
 */ 


#ifndef HD44780_H_
#define HD44780_H_

#include <avr/io.h>

/* эти параметры зависят от аппаратной конфигурации */
#define HD44780_RS_PIN              PC0
#define HD44780_RS_PORT             PORTC
#define HD44780_RW_PIN              PC2
#define HD44780_RW_PORT             PORTC
#define HD44780_E_PIN               PC1
#define HD44780_E_PORT              PORTC
#define HD44780_DATA_PORT           PORTC
#define HD44780_DATA_PIN            PC4
#define HD44780_DATA_WIDTH          4

/* установка лог. "1" на линии E */
#define HD44780_E_SET             ( HD44780_E_PORT  |= (1 << HD44780_E_PIN) )
/* установка лог. "0" на линии E */
#define HD44780_E_CLR             ( HD44780_E_PORT  &=~(1 << HD44780_E_PIN) )
/* установка лог. "1" на линии RS */
#define HD44780_RS_SET            ( HD44780_RS_PORT |= (1 << HD44780_RS_PIN) )
/* установка лог. "0" на линии RS */
#define HD44780_RS_CLR            ( HD44780_RS_PORT &=~(1 << HD44780_RS_PIN) )
/* задержка на передачу данных в контроллер ЖКИ */
#define HD44780_PUTNIBBLE_DELAY     _delay_us(50)

/* макрос, указывающий функции, что
передаются команды */
#define HD44780_COMMAND             0
/* макрос, указывающий функции, что
передаются данные */
#define HD44780_DATA                1

/* типы данных, используемые для инициализации дисплея */

/* физические параметры дисплея */
#define HD44780_PHYSICAL_CMD        (1 << 5)
#define HD44780_5x8MATRIX           (0 << 2)
#define HD44780_5x10MATRIX          (1 << 2)
#define HD44780_1ROW                (0 << 3)
#define HD44780_2ROW                (1 << 3)
#define HD44780_4BIT_BUS            (0 << 4)
#define HD44780_8BIT_BUS            (1 << 4)
/* поддержка русской кодовой страницы WS0001 */
#define HD44780_RUS_FONT_TABLE      (2 << 0)
#if (HD44780_DATA_WIDTH == 4)
#define HD44780_BUS_WIDTH   HD44780_4BIT_BUS
#elif (HD44780_DATA_WIDTH == 8)
#define HD44780_BUS_WIDTH   HD44780_8BIT_BUS
#else
#error Unsupported data bus width
#endif

typedef enum {
    Phys1Row5x8  = HD44780_PHYSICAL_CMD | HD44780_BUS_WIDTH | HD44780_1ROW | HD44780_5x8MATRIX  | HD44780_RUS_FONT_TABLE,
    Phys1Row5x10 = HD44780_PHYSICAL_CMD | HD44780_BUS_WIDTH | HD44780_1ROW | HD44780_5x10MATRIX | HD44780_RUS_FONT_TABLE,
    Phys2Row5x8  = HD44780_PHYSICAL_CMD | HD44780_BUS_WIDTH | HD44780_2ROW | HD44780_5x8MATRIX  | HD44780_RUS_FONT_TABLE,
    Phys2Row5x10 = HD44780_PHYSICAL_CMD | HD44780_BUS_WIDTH | HD44780_2ROW | HD44780_5x10MATRIX | HD44780_RUS_FONT_TABLE,
} Hd44780PhysicalParams_t;

/* направление вывода */
#define HD44780_OUT_CMD             (1 << 2)
#define HD44780_OUT_LEFT_TO_RIGHT   (1 << 1)
#define HD44780_OUT_RIGHT_TO_LEFT   (0 << 1)
typedef enum {
    OutNorm   = HD44780_OUT_CMD | HD44780_OUT_LEFT_TO_RIGHT,
    OutArabic = HD44780_OUT_CMD | HD44780_OUT_RIGHT_TO_LEFT
} Hd44780OutParams_t;

/* варианты отображения курсора */
#define HD44780_CURSOR_MODE_CMD     (1 << 3)
#define HD44780_DISPLAY_ON          (1 << 2)
#define HD44780_DISPLAY_OFF         (0 << 2)
#define HD44780_CURSOR_ON           (1 << 1)
#define HD44780_CURSOR_OFF          (0 << 1)
#define HD44780_BLINK_ON            (1 << 0)
#define HD44780_BLINK_OFF           (0 << 0)

typedef enum {
    DispOff          = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_OFF,
    CursorOff        = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF,
    CursorMode1      = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_ON  | HD44780_BLINK_OFF,
    CursorMode1Blink = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_ON  | HD44780_BLINK_ON,
    CursorMode2      = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF,
    CursorMode2Blink = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_ON
} Hd44780CursorMode_t;

#define HD44780_GOTO_CMD            (1 << 7)

/* функция передачи байта в жки. 
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
     rs = 1 - данные (сбрасывается линия RS) */
void hd44780_putbyte(char c, char rs);

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void hd44780_clear();

/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
void hd44780_gotoxy(char col, char row);

/* функция перемещения курсора в начальную
позицию (0, 0) */
void hd44780_home();

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void hd44780_init(Hd44780PhysicalParams_t ph,
                  Hd44780OutParams_t out,
                  Hd44780CursorMode_t cur);
              
/* функция вывода отдельного байта на текущей позиции */
static inline void hd44780_putc(char x) {
    hd44780_putbyte(x, HD44780_DATA);
}

/* функция вывода стандартной строки на текущей позиции */
void hd44780_puts(char * str);

#endif /* HD44780_H_ */