/*
 * spi.c
 *
 * Created: 16-Jul-15 23:18:34
 *  Author: Denis Vasilkovsky
 */ 

#include <avr/io.h>
#include "spi.h"

void spi_init(SpiConfig_t* config) {
    /* Установка выводов MOSI и SCK на вывод */
    DDRB = (1 << SPI_MOSI_PIN) | (1 << SPI_SCK_PIN);
	/* Если использован режим мастера, то нужно ножку SS либо
	переключить на вывод, либо поддерживать высокий уровень.
	В противном случае автоматически включиться режим ведомого*/
	if (config->master == SPI_MASTER_MODE) {
		DDRB |= (1 << SPI_SS_PIN);
	}
    /* Включение SPI, режима ведущего, и установка
	частоты тактирования fclk/128 */
    SPCR = (config->mode)|(config->clk)|(config->order)|(config->master);
}

/* Функция передачи байта данных outData и приема возвратного байта.*/
unsigned char spi_write_blocking(char outData) {
	/* Начало передачи */
	SPDR = outData;
	/* Ожидание окончания передачи */
	while(!(SPSR & (1<<SPIF))) ;
	return SPDR;
}
