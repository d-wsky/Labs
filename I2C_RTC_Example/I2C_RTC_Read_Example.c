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
#include "lcd_func.h"

/* Адрес микросхемы PCF8583 на шине I2C */
#define RTC_ADDR  0b10100000

/* инициализация портов, подключенных к жки */
void init_port()
{
    PORTC=0x00;
    DDRC=0xFF;
}

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

/* Функция вывода времени на ЖКИ */
void put_time(void) {
	/* Первая строка: время */
	lcd_gotoxy(0,0);
	/* Часы */
	lcd_putc(((hour>>4)&0x03)+0x30);
	lcd_putc((hour&0x0F)+0x30);
	lcd_putc(':');
	/* Минуты */
	lcd_putc((min>>4)+0x30);
	lcd_putc((min&0x0F)+0x30);
	lcd_putc(':');
	/* Секунды */
	lcd_putc((sec>>4)+0x30);
	lcd_putc((sec&0x0F)+0x30);
	/* Вторая строка: дата */
	lcd_gotoxy(0, 1);
	/* Число */
	lcd_putc(((year_date>>4)&0x03)+0x30);
	lcd_putc((year_date&0x0F)+0x30);
	lcd_putc('/');
	/* Месяц */
	lcd_putc(((day_month>>4)&0x03)+0x30);
	lcd_putc((day_month&0x0F)+0x30);
}

int main(void)
{
	init_port();
    lcd_init();
	lcd_clear();
	
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
	};
	
	return 0;
}