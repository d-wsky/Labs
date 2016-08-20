/*
 * mcp3204.h
 *
 * Created: 16-Jul-15 23:21:48
 *  Author: Denis Vasilkovsky
 */ 


#ifndef MCP3204_H_
#define MCP3204_H_

#include <avr/io.h>
#include <stdint.h>

/*! \brief ������� �����������, ������������ ����� CS. */
#define MCP3204_CS_DDR_REG         DDRB
/*! \brief ������� ������, ������������ ����� CS. */
#define MCP3204_CS_PORT_REG        PORTB
/*! \brief ����� �����, � ������� ��������� CS.*/
#define MCP3204_CS_PIN             PB0

/*! \brief ������� �������, � ������� ������������ ��������� ������
 * ��������������.*/
#define MCP3204_CHANNEL_POS              6
/*! \brief ������� �������, � ������� ������������ ����� �������� ���
 * ����������������� ��������������.*/
#define MCP3204_MODE_SINGLE_ENDED_POS    9
/*! \brief ������� ������� ��� ������� ������� ��������������.*/
#define MCP3204_START_POS               10
/*! \brief ���������� ������� ������� ��������������.*/
#define MCP3204_START                   (1 << MCP3204_START_POS)

/*! \brief � ������� ����� ���� ������ ������������ ����� �������� ���
 * ���������������� ���� ��������������.*/
typedef enum {
    MCP3204_MODE_SINGLE_ENDED  = 1 << MCP3204_MODE_SINGLE_ENDED_POS, /*!< ������� ������������ ��������������.*/
    MCP3204_MODE_DIFFERENTIAL  = 0 << MCP3204_MODE_SINGLE_ENDED_POS, /*!< ���������������� �������������.*/
} Mcp3204Mode_t;

/*! \brief � ������� ����� ���� ������ ������������ ����� 
 * ������ ��������������.*/
typedef enum {
    MCP3204_CHANNEL_CH0     = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< ������������ �����, ����� CH1.*/
    MCP3204_CHANNEL_CH1     = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< ������������ �����, ����� CH2.*/
    MCP3204_CHANNEL_CH2     = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< ������������ �����, ����� CH3.*/
    MCP3204_CHANNEL_CH3     = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< ������������ �����, ����� CH4.*/
    MCP3204_CHANNEL_CH0_CH1 = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< ���������������� �����, �������� (CH0-CH1).*/
    MCP3204_CHANNEL_CH1_CH0 = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< ���������������� �����, �������� (CH1-CH0).*/
    MCP3204_CHANNEL_CH2_CH3 = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< ���������������� �����, �������� (CH2-CH3).*/
    MCP3204_CHANNEL_CH3_CH2 = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< ���������������� �����, �������� (CH3-CH2).*/
} Mcp3204Channel_t;

/*! \brief ������� ������������� ������ ���.*/
void     mcp3204_init();
/*! \brief ������ �������������� �� ������������� ������ � ���������� ����������
 * ��� ����������.*/
uint16_t mcp3204_read(Mcp3204Channel_t channel);

#endif /* MCP3204_H_ */
