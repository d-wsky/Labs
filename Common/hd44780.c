/*
 * hd44780.c
 *
 * Created: 11-Aug-15 23:45:48
 *  Author: Денис Васильковский
 */ 

#include "hd44780.h"
#include <util/delay.h>

/* функция передачи тетрады в жки. передается
младшая тетрада входного байта */
void hd44780_putnibble(char t) {
    t <<= HD44780_DATA_PIN;
    HD44780_E_SET;
    const char port_mask1 = (1 << HD44780_DATA_PIN) - 1;
    const char port_mask2 = (1 << (HD44780_DATA_PIN + HD44780_DATA_WIDTH)) - 1;
    HD44780_DATA_PORT &=~(port_mask1 ^ port_mask2);
    HD44780_DATA_PORT |= t;
    HD44780_E_CLR;
    HD44780_PUTNIBBLE_DELAY;
}

/* функция передачи байта в жки. 
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
     rs = 1 - данные (сбрасывается линия RS) */
void hd44780_putbyte(char c, char rs) {
    if (rs == HD44780_COMMAND)
        HD44780_RS_CLR;
    else
        HD44780_RS_SET;
    #if (HD44780_DATA_WIDTH == 4)
    hd44780_putnibble(c >> 4);
    hd44780_putnibble(c);
    #else
    HD44780_E_SET;
    HD44780_DATA_PORT = c;
    HD44780_E_CLR;
    HD44780_PUTNIBBLE_DELAY;
    #endif
}

/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void hd44780_clear() {
    hd44780_putbyte(0x01, HD44780_COMMAND);
    _delay_us(1500);
}

/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
void hd44780_gotoxy(char col, char row) {
    char adr = 0x40 * (row & 1) + col;
    adr |= HD44780_GOTO_CMD;
    hd44780_putbyte(adr, HD44780_COMMAND);
}

/* функция перемещения курсора в начальную
позицию (0, 0) */
void hd44780_home() {
    hd44780_putbyte(0x02, HD44780_COMMAND);
}

static void port_init() {
    const char port_mask1 = (1 << HD44780_DATA_PIN) - 1;
    const char port_mask2 = (1 << (HD44780_DATA_PIN + HD44780_DATA_WIDTH)) - 1;
    volatile unsigned char* data_ddr = &HD44780_DATA_PORT - 1;
    *data_ddr |= port_mask1 ^ port_mask2;
    HD44780_DATA_PORT &=~(port_mask1 ^ port_mask2);
    
    volatile unsigned char* rs_ddr = &HD44780_RS_PORT - 1;
    *rs_ddr |= 1 << HD44780_RS_PIN;
    HD44780_RS_PORT &=~(1 << HD44780_RS_PIN);
    
    volatile unsigned char* e_ddr = &HD44780_E_PORT - 1;
    *e_ddr |= 1 << HD44780_E_PIN;
    HD44780_E_PORT &=~(1 << HD44780_E_PIN);
    
    volatile unsigned char* rw_ddr = &HD44780_RW_PORT - 1;
    *rw_ddr |= 1 << HD44780_RW_PIN;
    HD44780_RW_PORT &=~(1 << HD44780_RW_PIN);
}

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void hd44780_init(Hd44780PhysicalParams_t ph, 
              Hd44780OutParams_t out,
              Hd44780CursorMode_t cur) {
    port_init();
    _delay_ms(100);
    // Передача команды сброса дисплея WS0001
    HD44780_RS_CLR;
    hd44780_putnibble(0x00);
    _delay_ms(5);
    hd44780_putnibble(0x00);
    _delay_ms(5);
    hd44780_putnibble(0x00);
    _delay_ms(5);
    hd44780_putnibble(0x00);
    _delay_ms(5);
    hd44780_putnibble(0x00);
    _delay_ms(5);
    
    // Выбор шины, дисплея
    #if (HD44780_DATA_WIDTH == 4)
    hd44780_putnibble(ph >> 4);
    _delay_ms(4.1);
    hd44780_putnibble(ph >> 4);
    _delay_us(100);
    hd44780_putnibble(ph & 0x0f);
    // Здесь должна быть проверка флага занятости
    _delay_ms(10);
    #else
    hd44780_putbyte(ph, HD44780_COMMAND);
    _delay_ms(4.1);
    hd44780_putbyte(ph, HD44780_COMMAND);
    _delay_us(100);
    // Здесь должна быть проверка флага занятости
    _delay_ms(10);
    #endif    
    
    // Display ON/OFF Control
    hd44780_putbyte(CursorOff, HD44780_COMMAND);
    // Здесь должна быть проверка флага занятости
    _delay_ms(10);
    
    // Очистка дисплея
    hd44780_clear();
    // Здесь должна быть проверка флага занятости
    _delay_ms(10);

    // Режим вывода
    hd44780_putbyte(out, HD44780_COMMAND);
    
    // Дополнительно: режим работы курсора
    hd44780_putbyte(cur, HD44780_COMMAND);
    
    // Дополнительно: возврат в координату (0, 0)
    hd44780_home();
}

void hd44780_puts(char* str) {
    while (*str) {
        hd44780_putc(*str++);
    }
}