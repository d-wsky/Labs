/*
 * mcp492x.c
 *
 * Created: 27-Jul-15 21:43:25
 *  Author: d.wsky
 */ 

#include "mcp492x.h"

#define MCP492X_CS_ENABLE         MCP492X_CS_PORT_REG &= ~(1 << MCP492X_CS_PIN)
#define MCP492X_CS_DISABLE        MCP492X_CS_PORT_REG |=  (1 << MCP492X_CS_PIN)

// хранит конфигурацию между вызовами командами записи в ЦАП
static uint8_t internal_config;

void mcp492x_init()
{
	MCP492X_LDAC_DDR_REG |= 1 << MCP492X_LDAC_PIN;
	MCP492X_CS_DDR_REG   |= 1 << MCP492X_CS_PIN;
	
	MCP492X_LDAC_HIGH;
	MCP492X_CS_DISABLE;
	MCP492X_LDAC_LOW;
}

void mcp492x_set_config(Mcp492xConfig_t* config)
{
	internal_config = config->channel | config->buf | config->gain | config->shdn;
}

void mcp492x_write_data(uint16_t value)
{
	MCP492X_CS_ENABLE;
	value &= MCP492X_OUTPUT_MASK;
	value |= internal_config << 8;
	spi_write_blocking((uint8_t)(value >> 8));
	spi_write_blocking((uint8_t) value);
	MCP492X_CS_DISABLE;
}
