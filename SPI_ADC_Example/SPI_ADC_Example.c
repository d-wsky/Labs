/*
 * SPI_ADC_Example.c
 *
 * Created: 02.09.2011 17:40:39
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример взаимодействия с внейшней микросхемой АЦП MCP3204 по
 *          интерфейсу SPI. Также как и в примере со встроенным АЦП, полученные
 *          данные отображаются на портах C и B в двоичном виде. Не забываем,
 *          что для обеспечения связи с этой микросхемой необходимо установить
 *          правильные положения переключателей MOSI, MISO, SCK на плате
 *          UNI-DS3.
 *
 *    Note: На плате должны быть включены переключатели ADC-CS, MOSI, MISO, SCK.
 */ 

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>

#include "spi.h"
#include "mcp3204.h"

int main(void) {
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	SpiConfig_t spi_config = {
		.clk    = SPI_SCK_F_DIV_128,
		.master = SPI_MASTER_MODE,
		.mode   = SPI_MODE_00,
		.order  = SPI_DATA_ORDER_MSB_FIRST
	};
	
	spi_init(&spi_config);
	spi_enable();
	
	mcp3204_init();
	
    while(1) {
		uint16_t adc_result = mcp3204_read(MCP3204_CHANNEL_CH1);
		/* Отображение данных */
		PORTC = adc_result;
		PORTD = adc_result >> 8;
		
		_delay_ms(100);
    }
}