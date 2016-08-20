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

/*! \brief Регистр направления, используемый ножки CS. */
#define MCP3204_CS_DDR_REG         DDRB
/*! \brief Регистр данных, используемый ножки CS. */
#define MCP3204_CS_PORT_REG        PORTB
/*! \brief Номер ножки, к которой подключен CS.*/
#define MCP3204_CS_PIN             PB0

/*! \brief Битовая позиция, в которой производится настройка канала
 * преобразования.*/
#define MCP3204_CHANNEL_POS              6
/*! \brief Битовая позиция, в которой производится выбор обычного или
 * дифференциального преобразования.*/
#define MCP3204_MODE_SINGLE_ENDED_POS    9
/*! \brief Битовая позиция для команды запуска преобразования.*/
#define MCP3204_START_POS               10
/*! \brief Собственно команда запуска преобразования.*/
#define MCP3204_START                   (1 << MCP3204_START_POS)

/*! \brief С помощью этого типа данных производится выбор обычного или
 * дифференцильного типа преобразования.*/
typedef enum {
	MCP3204_MODE_SINGLE_ENDED  = 1 << MCP3204_MODE_SINGLE_ENDED_POS, /*!< Обычное однополярное преобразование.*/
	MCP3204_MODE_DIFFERENTIAL  = 0 << MCP3204_MODE_SINGLE_ENDED_POS, /*!< Дифференциальное преобразоване.*/
} Mcp3204Mode_t;

/*! \brief С помощью этого типа данных производится выбор 
 * канала преобразования.*/
typedef enum {
	MCP3204_CHANNEL_CH0     = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< Однополярный режим, канал CH1.*/
	MCP3204_CHANNEL_CH1     = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< Однополярный режим, канал CH2.*/
	MCP3204_CHANNEL_CH2     = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< Однополярный режим, канал CH3.*/
	MCP3204_CHANNEL_CH3     = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED, /*!< Однополярный режим, канал CH4.*/
	MCP3204_CHANNEL_CH0_CH1 = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< Дифференциальный режим, разность (CH0-CH1).*/
	MCP3204_CHANNEL_CH1_CH0 = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< Дифференциальный режим, разность (CH1-CH0).*/
	MCP3204_CHANNEL_CH2_CH3 = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< Дифференциальный режим, разность (CH2-CH3).*/
	MCP3204_CHANNEL_CH3_CH2 = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL, /*!< Дифференциальный режим, разность (CH3-CH2).*/
} Mcp3204Channel_t;

/*! \brief Функция инициализации модуля АЦП.*/
void     mcp3204_init();
/*! \brief Запуск преобразования по определенному каналу и последущее считывание
 * его результата.*/
uint16_t mcp3204_read(Mcp3204Channel_t channel);

#endif /* MCP3204_H_ */
