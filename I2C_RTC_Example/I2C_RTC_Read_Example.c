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

/* Адрес микросхемы PCF8583 на шине I2C */
#define RTC_ADDR  0b10100000

/* Передача команды СТАРТ на шину */
void I2C_StartCondition(void)
{
    /* Передача команды */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	
	/* Ожидание установки флага TWINT. Это будет
	означать окончание установки состояния СТАРТ */
	while (!(TWCR & (1<<TWINT)));

}

/* Передача байта по шине с установлением
подтверждения (ACK)*/
void I2C_SendByte(unsigned char c)
{
	/* Запись данных в регистр TWDR. Очистка бита
	TWINT для начала передачи */
    TWDR = c;
    TWCR = (1<<TWINT)|(1<<TWEN);
	
	/* Ожидание установки флага TWINT. Это будет
	означать окончание передачи и установку
	подтверждения (ACK) */
	while (!(TWCR & (1<<TWINT)));
}

/* Получение байта */
unsigned char I2C_RecieveByte(void)
{
	unsigned char dat;
    
	/* Запуск процесса */
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);

	/* Ожидание установки флага TWINT, говорящем
	об окончании передачи.*/
	while (!(TWCR & (1<<TWINT)));
	
	/* Считывание данных */
	dat = TWDR;
	return dat;
}

/* Получение байта без установки подтверждения */
unsigned char I2C_RecieveLastByte(void)
{
	unsigned char dat;
	/* Запуск процесса */
	TWCR = (1<<TWINT)|(1<<TWEN)|(0<<TWEA);

	/* Ожидание установки флага TWINT, говорящем
	об окончании передачи.*/
	while (!(TWCR & (1<<TWINT)));
	
	dat = TWDR;
	return dat;
}

/* Передача состояния СТОП на шину */
void I2C_StopCondition(void) {
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

/* Глобальные переменные, хранящие текущее время и дату */
static unsigned char sec, min, hour,
                     year_date, day_month;

/* Считывание текущего времени из микросхемы I2C */
void get_time(void) {
	I2C_StartCondition();
	I2C_SendByte(RTC_ADDR);
	I2C_SendByte(0x02);
	I2C_StartCondition();
	I2C_SendByte(RTC_ADDR|1);
	sec=I2C_RecieveByte();
	min=I2C_RecieveByte();
	hour=I2C_RecieveByte();
	year_date=I2C_RecieveByte();
	day_month=I2C_RecieveLastByte();
	I2C_StopCondition();
}

/* переводит входное число в формате BCD в сторку
str длиной два символа с ведущим нулем */
void bcd_to_str(uint8_t num, char* str) {
	str[0] = (num >> 4) + 0x30;
	str[1] = (num & 0x0F) + 0x30;
}

/* Функция вывода времени на ЖКИ */
void put_time(void) {
	/* Первая строка: время */
	hd44780_gotoxy(0,0);
	char buf[4] = {' ', ' ', ':', '\0'};
	/* Часы */
	bcd_to_str(hour, buf);
	hd44780_puts(buf);
	/* Минуты */
	bcd_to_str(min, buf);
	hd44780_puts(buf);
	/* Секунды */
	bcd_to_str(sec, buf);
	buf[2] = '\0';
	hd44780_puts(buf);
	/* Вторая строка: дата */
	hd44780_gotoxy(0, 1);
	buf[2] = '/';
	/* Число */
	bcd_to_str(year_date & 0x03, buf);
	hd44780_puts(buf);
	/* Месяц */
	bcd_to_str(day_month & 0x03, buf);
	buf[2] = '\0';
	hd44780_puts(buf);
}

int main(void)
{
	hd44780_init(Phys2Row5x8, OutNorm, CursorMode2);
	hd44780_clear();
	
	/* Инициализация обмена по TWI на 100 кГц */
    TWBR=0x2A;
    /* Инициализация адреса МК на шине I2C */
    TWAR=0x00;
    
    while (1) {
		/* Считывание времени */
		get_time();
		
		/* Вывод времени в формате:
		 HH:MM:SS
		 DD/MM */
		put_time();
		
		_delay_ms(500);
	};
	
	return 0;
}