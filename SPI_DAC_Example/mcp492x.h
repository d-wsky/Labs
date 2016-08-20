/*
 * mcp4921.h
 *
 * Created: 27-Jul-15 00:40:39
 *  Author: d.wsky
 */ 


#ifndef MCP4921_H_
#define MCP4921_H_

#include "spi.h"

/*! \brief ������ �������������� ����� CS ����.*/
#define MCP492X_CS_PORT_REG       PORTB
#define MCP492X_CS_DDR_REG        DDRB
#define MCP492X_CS_PIN            PB4

/*! \brief ������ �������������� ����� CS ����.*/
#define MCP492X_LDAC_PORT_REG     PORTB
#define MCP492X_LDAC_DDR_REG      DDRB
#define MCP492X_LDAC_PIN          PB5

/*! \brief ������� ���������� ������ LDAC.*/
#define MCP492X_LDAC_HIGH         MCP492X_LDAC_PORT_REG |=  (1 << MCP492X_LDAC_PIN)
#define MCP492X_LDAC_LOW          MCP492X_LDAC_PORT_REG &= ~(1 << MCP492X_LDAC_PIN)
#define MCP492X_LDAC_SEND         do { MCP492X_LDAC_LOW; MCP492X_LDAC_HIGH; } while (0)

/* ������������� ������� ������� ��������� ������ ������� � ���.*/
#define MCP492X_CHANNEL_POS       7
#define MCP492X_BUFFER_POS        6
#define MCP492X_OUTGAIN_POS       5
#define MCP492X_SHDN_POS          4
/* ����� �� �������� ������ ���.*/
#define MCP492X_OUTPUT_MASK       0xFFF

/*! \brief � ������� ����� ���� ������ ����� ������� ����� ��������������.*/
typedef enum {
    MCP492X_CHANNEL_A = 0 << MCP492X_CHANNEL_POS,  /*!< ��� MCP4921 �������� ������ ���� �����.*/
    MCP492X_CHANNEL_B = 1 << MCP492X_CHANNEL_POS,  /*!< ��� MCP4922 �������� ����� ����� ��������.*/
} Mcp492xChannel_t;

/*! \brief � ������� ����� ���� ������ ����� ������� ������������ ��� ���
 * ���������� ����� ���.*/
typedef enum {
    MCP492X_BUFFER_OFF = 0 << MCP492X_BUFFER_POS,  /*!< ������� ������ ����� ��������� �� �����.*/
    MCP492X_BUFFER_ON  = 1 << MCP492X_BUFFER_POS   /*!< ������� ������ ��������� �� ����� ��� ������ ������ �� LDAC.*/
} Mcp492xBuffer_t;

/*! \brief � ������� ����� ���� ������ ����� ������� ������������ ��� ���
 * �������� ������������ ������ ���.*/
typedef enum {
    MCP492X_OUTGAIN_1x = 1 << MCP492X_OUTGAIN_POS, /*!< Vout = Vref * D / 4096.*/
    MCP492X_OUTGAIN_2x = 0 << MCP492X_OUTGAIN_POS, /*!< Vout = 2 * Vref * D / 4096.*/
} Mcp492xGain_t;

/*! \brief � ������� ����� ���� ������ ����� ������, �������� �� ����������
 * ���� ����.*/
typedef enum {
    MCP492X_SHDN_OFF = 1 << MCP492X_SHDN_POS,      /*!< ��� ������� � ������.*/
    MCP492X_SHDN_ON  = 0 << MCP492X_SHDN_POS,      /*!< ��� ��������, ����� � ������� ���������.*/
} Mcp492xShutdown_t;

/*! \brief ������������ ���.*/
typedef struct {
    Mcp492xChannel_t  channel;
    Mcp492xBuffer_t   buf;
    Mcp492xGain_t     gain;
    Mcp492xShutdown_t shdn;
} Mcp492xConfig_t;

/*! \brief ������������� ���.

������ ��� ��������� ������ � ���, ���������� ����� ��� ����������������
� ������ � ������� ��������������� �������.*/
void mcp492x_init();

/*! \brief ��������� ������������ ���.
 *
 * �� ���������� ����������������� ��������� ������������ ���������� �� ��� ���,
 * ���� �� ����� �������� ����� ������ � ��� � ������� ��������������� �������.*/
void mcp492x_set_config(Mcp492xConfig_t* config);

/*! \brief ������ ����� ������ � ���.
 *
 * ������ � ������� ������������ � ����� ������������.*/
void mcp492x_write_data(uint16_t value);

#endif /* MCP4921_H_ */