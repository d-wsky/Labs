/*
 * SPI_DAC_Example.c
 *
 * Created: 23.10.2011 18:28:15
 *  Edited: 28.07.2015 01:13:12
 *  Author: Denis Vasilkovsky
 *
 *   About: Пример взаимодействия с внейшней микросхемой ЦАП MCP4921 по
 *          интерфейсу SPI. МК последовательно устанавливает 5 разных значений
 *          напряжения с интервалом в 2 секунды. Чтобы добавить немного интерактивчика
 *          дополнительно загораются светодиоды PA-PA5.
 *
 *    Note: На плате должны быть включены переключатели DAC-CS, DAC-LD, MOSI, SCK.
 */

#include <avr/io.h>
#define F_CPU 10000000UL
#include <util/delay.h>
#include "spi.h"
#include "mcp492x.h"

int main(void)
{
    int t = 0;
    DDRB = 0xFF;
    
    SpiConfig_t spi_config = {
        .master = SPI_MASTER_MODE,
        .clk    = SPI_SCK_F_DIV_4,
        .mode   = SPI_MODE_00,
        .order  = SPI_DATA_ORDER_MSB_FIRST,
    };

    spi_init(&spi_config);
    spi_enable();
    
    mcp492x_init();
    Mcp492xConfig_t dac_config = {
        .buf     = MCP492X_BUFFER_OFF,
        .channel = MCP492X_CHANNEL_A,
        .gain    = MCP492X_OUTGAIN_1x,
        .shdn    = MCP492X_SHDN_OFF,
    };
    mcp492x_set_config(&dac_config);
    
    DDRC = 0xFF;
    DDRA = 0xff;
    PORTA = 1;

    while (1) {
        t += 1023;
        PORTA <<= 1;
        if (t > 4095) {
            t = 0;
            PORTA = 1;
        }
        mcp492x_write_data(t);
        _delay_ms(2000);
    }
}