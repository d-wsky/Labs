/*
 * mcp3204.h
 *
 * Created: 16-Jul-15 23:21:48
 *  Author: Denis Vasilkovsky
 */ 


#ifndef MCP3204_H_
#define MCP3204_H_

#include <stdint.h>

/*! \brief Регистр направления, используемый ножки CS. */
#define MCP3204_CS_DDR_REG         DDRB
/*! \brief Регистр данных, используемый ножки CS. */
#define MCP3204_CS_PORT_REG        PORTB
/*! \brief Номер ножки, к которой подключен CS.*/
#define MCP3204_CS_PIN             PB0

#define MCP3204_CHANNEL_POS              6
#define MCP3204_MODE_SINGLE_ENDED_POS    9
#define MCP3204_START_POS               10
#define MCP3204_START                   (1 << MCP3204_START_POS)

typedef enum {
	MCP3204_MODE_SINGLE_ENDED  = 1 << MCP3204_MODE_SINGLE_ENDED_POS,
	MCP3204_MODE_DIFFERENTIAL  = 0 << MCP3204_MODE_SINGLE_ENDED_POS,
} Mcp3204Mode_t;

typedef enum {
	MCP3204_CHANNEL_CH0     = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED,
	MCP3204_CHANNEL_CH1     = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED,
	MCP3204_CHANNEL_CH2     = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED,
	MCP3204_CHANNEL_CH3     = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_SINGLE_ENDED,
	MCP3204_CHANNEL_CH0_CH1 = (0 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL,
	MCP3204_CHANNEL_CH1_CH0 = (1 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL,
	MCP3204_CHANNEL_CH2_CH3 = (2 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL,
	MCP3204_CHANNEL_CH3_CH2 = (3 << MCP3204_CHANNEL_POS) | MCP3204_MODE_DIFFERENTIAL,
} Mcp3204Channel_t;

void     mcp3204_init();
uint16_t mcp3204_read(Mcp3204Channel_t channel);

#endif /* MCP3204_H_ */