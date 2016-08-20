/*
 * I2C_RTC_Example.c
 *
 * Created: 13.09.2011 22:19:34
 *  Author: Денис Васильковский
 *
 *   About: Пример использования шины I2C для получения данных с внешней
 *          микросхемы часов реального времени. После старта МК отображает
 *          текущее время и дату на знакоместном ЖКИ, подключенном по
 *          стандартному интерфейсу UNI-DS3.
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "i2c.h"
#include "pcf8583.h"

#define I2C_FREQ  100000
#define LCD_WIDTH 16

/* переводит входное число в формате BCD в сторку
str длиной два символа с ведущим нулем */
void bcd_to_str(uint8_t num, char* str) {
    str[0] = (num >> 4) + 0x30;
    str[1] = (num & 0x0F) + 0x30;
}

/* Функция вывода времени на ЖКИ */
void put_time(pcf8583_time_t* t) {
    hd44780_gotoxy(0, 0);
    /* Первая строка: время */
    {
        char buf[] = {' ', ' ', ':', ' ', ' ', ':', ' ', ' ', '.', ' ', ' ', '\0'};
        const size_t HOUR_POS = 0;
        const size_t MIN_POS = 3;
        const size_t SEC_POS = 6;
        const size_t HSEC_POS = 9;
        bcd_to_str(t->hour, &buf[HOUR_POS]);
        bcd_to_str(t->min,  &buf[MIN_POS]);
        bcd_to_str(t->sec,  &buf[SEC_POS]);
        bcd_to_str(t->hsec, &buf[HSEC_POS]);
        hd44780_puts(buf);
    }
    /* Вторая строка: дата */
    hd44780_gotoxy(0, 1);
    {
        char buf[LCD_WIDTH] = {' ', ' ', '/', ' ', ' ', '/', ' ', ' ', '\0'};
        const size_t DATE_POS = 0;
        const size_t MONTH_POS = 3;
        const size_t YEAR_POS = 6;
        bcd_to_str(t->date,  &buf[DATE_POS]);
        bcd_to_str(t->month, &buf[MONTH_POS]);
        bcd_to_str(t->year,  &buf[YEAR_POS]);
        hd44780_puts(buf);
    }
}

int main(void)
{
     hd44780_init(Phys2Row5x8, OutNorm, CursorMode2);
    hd44780_clear();
    
    i2c_init(I2C_FREQ);
        
    while (1) {
        pcf8583_time_t t;
        /* Считывание времени */
        pcf8583_read_time(&t);
        
        /* Вывод времени */
        put_time(&t);
        
        /* Пауза */
        _delay_ms(500);
    };
    
    return 0;
}
