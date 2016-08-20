/*
 * mcp3204.c
 *
 * Created: 16-Jul-15 23:33:59
 *  Author: Denis Vasilkovsky
 */ 

#include "mcp3204.h"
#include "spi.h"

/*! \brief ������ ������� �������� ����� CS ��� � ��������� ���. "1" */
#define MPC3204_CS_HIGH  (MCP3204_CS_PORT_REG |=  (1 << MCP3204_CS_PIN))
/*! \brief ������ ������� �������� ����� CS ��� � ��������� ���. "0" */
#define MCP3204_CS_LOW   (MCP3204_CS_PORT_REG &= ~(1 << MCP3204_CS_PIN))

void mcp3204_init() {
    /* ���������� ����� CS */
    MCP3204_CS_DDR_REG |= (1 << MCP3204_CS_PIN); 
    
    /* ���� ��� ��� ������� � ������ CS �
    ��������� ���. "0", �� ����� ������������
    �� � �������� � ������� ��������� ���. "1" */
    MPC3204_CS_HIGH;
    MCP3204_CS_LOW;
    MPC3204_CS_HIGH;
};

uint16_t mcp3204_read(Mcp3204Channel_t channel) {
    /* ���������� ��������� ������ */
    uint16_t cfg;
    cfg = MCP3204_START | channel;
    
    /* �������� ������ */
    MCP3204_CS_LOW;
    spi_write_blocking((uint8_t)(cfg >> 8));
    uint16_t result = spi_write_blocking((uint8_t)cfg);
    result   = (result & 0xF) << 8;
    result  |= spi_write_blocking(0) & 0xFF;
    MPC3204_CS_HIGH;
    
    return result;
}
