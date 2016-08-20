/*
 * spi.c
 *
 * Created: 16-Jul-15 23:18:34
 *  Author: Denis Vasilkovsky
 */ 

#include <avr/io.h>
#include "gpio.h"
#include "spi.h"

/* ��������������� ������, ������� ������ ��������� ��������,
����� ���������� ������� ������-��������.*/
#define TRX_COMPLETE   ( SPSR & (1 << SPIF) )

void spi_init(SpiConfig_t * config) {
    /* ��������� ������� MOSI � SCK �� ����� */
    gpio_pin_mode_out(SPI_PORT, (1 << SPI_MOSI_PIN) | (1 << SPI_SCK_PIN));
    /* ���� ����������� ����� �������, �� ����� ����� SS ����
    ����������� �� �����, ���� ������������ ������� �������.
    � ��������� ������ ������������� ���������� ����� ��������*/
    if (config->master == SPI_MASTER_MODE) {
        gpio_pin_mode_out(SPI_PORT, (1 << SPI_SS_PIN));
    }
    /* ��������� ������������ ������ SPI */
    SPCR = (config->mode) | (config->clk) | (config->order) | (config->master);
}

/* ������� �������� ����� ������ outData � ������ ����������� �����.*/
unsigned char spi_write_blocking(char outData) {
    /* ������ �������� */
    SPDR = outData;
    /* �������� ��������� �������� */
    while (!TRX_COMPLETE) {
        // ������ ����
    }
    return SPDR;
}
