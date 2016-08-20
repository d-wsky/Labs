/*
 * SPI_ADC_Example.c
 *
 * Created: 02.09.2011 17:40:39
 *  Author: Denis Vasilkovsky
 *
 *   About: ������ �������������� � �������� ����������� ��� MCP3204 ��
 *          ���������� SPI. ����� ��� � � ������� �� ���������� ���, ����������
 *          ������ ������������ �� ������ C � B � �������� ����. �� ��������,
 *          ��� ��� ����������� ����� � ���� ����������� ���������� ����������
 *          ���������� ��������� �������������� MOSI, MISO, SCK, ADC-CS �� �����
 *          UNI-DS3.
 *
 *    Note: �� ����� ������ ���� �������� ������������� ADC-CS, MOSI, MISO, SCK.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

#include "gpio.h"
#include "spi.h"
#include "mcp3204.h"

int main(void) {
    /* 0. ���������� ������������ ������� � ������.*/
    
    /* �� ��� ����� ��������� ��������� ��������������.*/
    gpio_pin_mode_out(GPIO_PORTC, GPIO_PIN_ALL);
    gpio_pin_mode_out(GPIO_PORTD, GPIO_PIN_ALL);
    
    /* ���������� ������ ���������� SPI � ������.*/
    SpiConfig_t spi_config = {
        .clk    = SPI_SCK_F_DIV_128,
        .master = SPI_MASTER_MODE,
        .mode   = SPI_MODE_00,
        .order  = SPI_DATA_ORDER_MSB_FIRST
    };
    
    spi_init(&spi_config);
    spi_enable();
    
    /* ������������� ���������� ���.*/
    mcp3204_init();
    
    while(1) {
        /* 1. ������ ������.*/
        uint16_t adc_result = mcp3204_read(MCP3204_CHANNEL_CH1);

        /* 2. ����������� ������.*/
        gpio_port_assign(GPIO_PORTC, adc_result);
        gpio_port_assign(GPIO_PORTD, adc_result >> 8);
        
        /* 3. �����.*/
        _delay_ms(100);
    }
}
