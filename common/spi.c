/*
 * spi.c
 *
 * Created: 16-Jul-15 23:18:34
 *  Author: Denis Vasilkovsky
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "spi.h"

/* Вспомогательный макрос, который вернет ненулевое значение,
когда завершится текущая приемо-передача.*/
#define TRX_COMPLETE   ( SPSR & (1 << SPIF) )

void spi_init(SpiConfig_t * config) {
    /* Установка выводов MOSI и SCK на вывод */
    gpio_pin_mode_out(SPI_PORT, (1 << SPI_MOSI_PIN) | (1 << SPI_SCK_PIN));
    /* Если использован режим мастера, то нужно ножку SS либо
    переключить на вывод, либо поддерживать высокий уровень.
    В противном случае автоматически включиться режим ведомого*/
    if (config->master == SPI_MASTER_MODE) {
        gpio_pin_mode_out(SPI_PORT, (1 << SPI_SS_PIN));
    }
    /* Включение запрошенного режима SPI */
    SPCR = (config->mode) | (config->clk) | (config->order) | (config->master);
}

/* Функция передачи байта данных outData и приема возвратного байта.*/
unsigned char spi_write_blocking(char outData) {
    /* Начало передачи */
    SPDR = outData;
    /* Ожидание окончания передачи */
    while (!TRX_COMPLETE) {
        // Пустой цикл
    }
    return SPDR;
}
