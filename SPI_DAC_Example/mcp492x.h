/*
 * mcp4921.h
 *
 * Created: 27-Jul-15 00:40:39
 *  Author: d.wsky
 */ 


#ifndef MCP4921_H_
#define MCP4921_H_

#include "spi.h"

/*! \brief Задает местоположение ножки CS ЦАПа.*/
#define MCP492X_CS_PORT_REG       PORTB
#define MCP492X_CS_DDR_REG        DDRB
#define MCP492X_CS_PIN            PB4

/*! \brief Задает местоположение ножки CS ЦАПа.*/
#define MCP492X_LDAC_PORT_REG     PORTB
#define MCP492X_LDAC_DDR_REG      DDRB
#define MCP492X_LDAC_PIN          PB5

/*! \brief Команды управления ножкой LDAC.*/
#define MCP492X_LDAC_HIGH         MCP492X_LDAC_PORT_REG |=  (1 << MCP492X_LDAC_PIN)
#define MCP492X_LDAC_LOW          MCP492X_LDAC_PORT_REG &= ~(1 << MCP492X_LDAC_PIN)
#define MCP492X_LDAC_SEND         do { MCP492X_LDAC_LOW; MCP492X_LDAC_HIGH; } while (0)

/* Специфические битовые позиции протокола обмена данными с ЦАП.*/
#define MCP492X_CHANNEL_POS       7
#define MCP492X_BUFFER_POS        6
#define MCP492X_OUTGAIN_POS       5
#define MCP492X_SHDN_POS          4
/* Маска на выходные данные ЦАП.*/
#define MCP492X_OUTPUT_MASK       0xFFF

/*! \brief С помощью этого типа данных можно выбрать канал преобразования.*/
typedef enum {
    MCP492X_CHANNEL_A = 0 << MCP492X_CHANNEL_POS,  /*!< Для MCP4921 доступен только этот канал.*/
    MCP492X_CHANNEL_B = 1 << MCP492X_CHANNEL_POS,  /*!< Для MCP4922 возможен выбор между каналами.*/
} Mcp492xChannel_t;

/*! \brief С помощью этого типа данных можно выбрать использовать или нет
 * внутренний буфер ЦАП.*/
typedef enum {
    MCP492X_BUFFER_OFF = 0 << MCP492X_BUFFER_POS,  /*!< Входные данные сразу поступают на выход.*/
    MCP492X_BUFFER_ON  = 1 << MCP492X_BUFFER_POS   /*!< Входные данные поступают на выход при низком уровне на LDAC.*/
} Mcp492xBuffer_t;

/*! \brief С помощью этого типа данных можно выбрать использовать или нет
 * выходной усилительный каскад ЦАП.*/
typedef enum {
    MCP492X_OUTGAIN_1x = 1 << MCP492X_OUTGAIN_POS, /*!< Vout = Vref * D / 4096.*/
    MCP492X_OUTGAIN_2x = 0 << MCP492X_OUTGAIN_POS, /*!< Vout = 2 * Vref * D / 4096.*/
} Mcp492xGain_t;

/*! \brief С помощью этого типа данных можно решить, включены ли аналоговые
 * цепи ЦАПа.*/
typedef enum {
    MCP492X_SHDN_OFF = 1 << MCP492X_SHDN_POS,      /*!< ЦАП включен в работу.*/
    MCP492X_SHDN_ON  = 0 << MCP492X_SHDN_POS,      /*!< ЦАП отключен, выход в третьем состоянии.*/
} Mcp492xShutdown_t;

/*! \brief Конфигурация ЦАП.*/
typedef struct {
    Mcp492xChannel_t  channel;
    Mcp492xBuffer_t   buf;
    Mcp492xGain_t     gain;
    Mcp492xShutdown_t shdn;
} Mcp492xConfig_t;

/*! \brief Инициализация ЦАП.

Прежде чем выполнять запись в ЦАП, желательно также его сконфигурировать
в работу с помощью соответствующей функции.*/
void mcp492x_init();

/*! \brief Изменение конфигурации ЦАП.
 *
 * Не производит непосредственного изменения конфигурации микросхемы до тех пор,
 * пока не будут записаны новые данные в ЦАП с помощью соответствующей функции.*/
void mcp492x_set_config(Mcp492xConfig_t* config);

/*! \brief Запись новых данных в ЦАП.
 *
 * Вместе с данными отправляется и новая конфигурация.*/
void mcp492x_write_data(uint16_t value);

#endif /* MCP4921_H_ */