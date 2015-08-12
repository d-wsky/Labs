/*
 * I2C_RTC_Example.c
 *
 * Created: 13.09.2011 22:19:34
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример использования шины I2C для получения данных с внешней
 *          микросхемы часов реального времени. После старта МК отображает
 *          текущее время и дату на знакоместном ЖКИ, подключенном по
 *          стандартному интерфейсу UNI-DS3.
 *
 *    TODO: Перенести код I2C и RTC в отдельные файлы. И исправить стремные
 *          названия функций get_time, put_time.
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"
#include "i2c.h"

/* Адрес микросхемы PCF8583 на шине I2C */
#define RTC_ADDR  0b10100000

#define I2C_FREQ  100000

/* Глобальные переменные, хранящие текущее время и дату */
typedef struct {
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char year_date;
	unsigned char day_month;
} time_t ;

/* Считывание текущего времени из микросхемы I2C */
void get_time(time_t* t) {
	i2c_start();
	i2c_write_addr(RTC_ADDR);
	i2c_write_byte(0x02);
	i2c_start();
	i2c_write_addr(RTC_ADDR|1);
	i2c_read_multiple((unsigned char*)t, sizeof(time_t));
	i2c_stop();
}

/* переводит входное число в формате BCD в сторку
str длиной два символа с ведущим нулем */
void bcd_to_str(uint8_t num, char* str) {
	str[0] = (num >> 4) + 0x30;
	str[1] = (num & 0x0F) + 0x30;
}

/* Функция вывода времени на ЖКИ */
void put_time(time_t* t) {
	/* Первая строка: время */
	hd44780_gotoxy(0,0);
	char buf[4] = {' ', ' ', ':', '\0'};
	/* Часы */
	bcd_to_str(t->hour, buf);
	hd44780_puts(buf);
	/* Минуты */
	bcd_to_str(t->min, buf);
	hd44780_puts(buf);
	/* Секунды */
	bcd_to_str(t->sec, buf);
	buf[2] = '\0';
	hd44780_puts(buf);
	/* Вторая строка: дата */
	hd44780_gotoxy(0, 1);
	buf[2] = '/';
	/* Число */
	bcd_to_str(t->year_date & 0x03, buf);
	hd44780_puts(buf);
	/* Месяц */
	bcd_to_str(t->day_month & 0x03, buf);
	buf[2] = '\0';
	hd44780_puts(buf);
}

int main(void)
{
	hd44780_init(Phys2Row5x8, OutNorm, CursorMode2);
	hd44780_clear();
	
	i2c_init(I2C_FREQ);
    
    while (1) {
		time_t t;
		/* Считывание времени */
		get_time(&t);
		
		/* Вывод времени в формате:
		 HH:MM:SS
		 DD/MM */
		put_time(&t);
		
		_delay_ms(500);
	};
	
	return 0;
}