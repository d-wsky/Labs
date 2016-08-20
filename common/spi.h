/*
 * spi.h
 *
 * Created: 16-Jul-15 23:17:37
 *  Author: Denis Vasilkovsky
 *
 *   About: ������� ������� ��� ������ SPI. ��������� ���������� �������������
 *          ������ � �������� ������ � ����������� (����������) ����.
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

// ����, � �������� ���������� ����� SPI
#define SPI_PORT          GPIO_PORTB
// ����� ����� MOSI
#define SPI_MOSI_PIN      GPIO_PIN2
// ����� ����� MISO
#define SPI_SCK_PIN       GPIO_PIN1
// ����� ����� SS
#define SPI_SS_PIN        GPIO_PIN0

/*! \brief ��� ������, ������������ ��� ������ ������ �������-�������.*/
typedef enum {
    SPI_MASTER_MODE  =   (1 << MSTR), /*!< ����� ��������.*/
    SPI_SLAVE_MODE   =   (0 << MSTR)  /*!< ����� �������� (experimental).*/
} SpiMasterSlaveMode_t;

/*! \brief ��� ������, ������������ ��� ������ �������� �������� ����������.*/
typedef enum {
    SPI_SCK_F_DIV_4   =   (0 << SPR1) | (0 << SPR0), /*!< F_CPU/4.*/
    SPI_SCK_F_DIV_16  =   (0 << SPR1) | (1 << SPR0), /*!< F_CPU/16.*/
    SPI_SCK_F_DIV_64  =   (1 << SPR1) | (0 << SPR0), /*!< F_CPU/64.*/
    SPI_SCK_F_DIV_128 =   (1 << SPR1) | (1 << SPR0)  /*!< F_CPU/128.*/
} SpiClock_t;

/*! \brief ��� ������, ������������ ��� ������ ������ ������������ ������.*/
typedef enum {
    SPI_MODE_00       =   (0 << CPOL) | (0 << CPHA), /*!< �������� ������� - ������, ��������� �� ������� ������, ������� - �� ���������.*/
    SPI_MODE_01       =   (0 << CPOL) | (1 << CPHA), /*!< �������� ������� - ������, ��������� �� ��������� ������, ������� - �� �������.*/
    SPI_MODE_10       =   (1 << CPOL) | (0 << CPHA), /*!< �������� ������� - �������, ��������� �� ������� ������, ������� - �� ���������.*/
    SPI_MODE_11       =   (1 << CPOL) | (1 << CPHA), /*!< �������� ������� - �������, ��������� �� ��������� ������, ������� - �� �������.*/
} SpiMode_t;

/*! \brief ��� ������, ������������ ��� ������ ������� �������� ������
 * � �����.*/
typedef enum {
    SPI_DATA_ORDER_MSB_FIRST =  (0 << DORD), /*!< ������ ������� ������� ���.*/
    SPI_DATA_ORDER_LSB_FIRST =  (1 << DORD), /*!< ������ ������� ������� ���.*/
} SpiDataOrder_t;

/*! \brief ���������, ���������� ������ ������������ ����������� ������ SPI.*/
typedef struct {
    SpiMasterSlaveMode_t  master; /*!< �������-�������.*/
    SpiClock_t            clk;    /*!< ������� ������������ ������.*/
    SpiMode_t             mode;   /*!< ����� ������������.*/
    SpiDataOrder_t        order;  /*!< ������� �������� ���.*/
} SpiConfig_t;

/*! \brief ������� ������������� ���� SPI � ������ �������.*/
void spi_init(SpiConfig_t* config);

/*! \brief ������� ��������� ����������������� � ������.*/
static inline void spi_enable()  { SPCR |=  (1 << SPE); }

/*! \brief ������� ���������� �����������������.*/
static inline void spi_disable() { SPCR &= ~(1 << SPE); }

/*! \brief ������� �������� ����� ������ outData.
 *
 * ���������� ����������� �������� ��������� �������� � ���������� ��������
 * ������� �� ����� MOSI ����.
 *
 * \return ���������� �������� � ������ �������� ����.*/
unsigned char spi_write_blocking(char outData);

#endif /* SPI_H_ */
