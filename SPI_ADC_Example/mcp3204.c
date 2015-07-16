/*
 * mcp3204.c
 *
 * Created: 16-Jul-15 23:33:59
 *  Author: Denis Vasilkovsky
 */ 

#include "mcp3204.h"
#include "spi.h"

/*! \brief Макрос команды перевода ножки CS АЦП в состояние лог. "1" */
#define MPC3204_CS_HIGH  (MCP3204_CS_PORT_REG |=  (1 << MCP3204_CS_PIN))
/*! \brief Макрос команды перевода ножки CS АЦП в состояние лог. "0" */
#define MCP3204_CS_LOW   (MCP3204_CS_PORT_REG &= ~(1 << MCP3204_CS_PIN))

void mcp3204_init() {
	/* Подготовка ножки CS */
	MCP3204_CS_DDR_REG |= (1 << MCP3204_CS_PIN); 
	
	/* Если АЦП был запущен с ножкой CS в
	состоянии лог. "0", то нужно перещелкнуть
	ее и оставить в рабочем положении лог. "1" */
	MPC3204_CS_HIGH;
	MCP3204_CS_LOW;
	MPC3204_CS_HIGH;
};

uint16_t mcp3204_read(Mcp3204Channel_t channel) {
	/* Заполнение исходящих данных */
	uint16_t cfg;
	cfg = MCP3204_START | channel;
	
	/* Передача данных */
	MCP3204_CS_LOW;
	spi_write_blocking((uint8_t)(cfg >> 8));
	uint16_t result = spi_write_blocking((uint8_t)cfg);
	result   = (result & 0xF) << 8;
	result  |= spi_write_blocking(0) & 0xFF;
	MPC3204_CS_HIGH;
	
	return result;
}
