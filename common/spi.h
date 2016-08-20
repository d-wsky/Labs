/*
 * spi.h
 *
 * Created: 16-Jul-15 23:17:37
 *  Author: Denis Vasilkovsky
 *
 *   About: Простой драйвер для модуля SPI. Позволяет произвести инициализацию
 *          модуля и передать данные в блокирующем (синхронном) виде.
 */

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

// Порт, к которому подключены ножки SPI
#define SPI_PORT          GPIO_PORTB
// Вывод ножки MOSI
#define SPI_MOSI_PIN      GPIO_PIN2
// Вывод ножки MISO
#define SPI_SCK_PIN       GPIO_PIN1
// Вывод ножки SS
#define SPI_SS_PIN        GPIO_PIN0

/*! \brief Тип данных, используемый для выбора режима ведущий-ведомый.*/
typedef enum {
    SPI_MASTER_MODE  =   (1 << MSTR), /*!< Режим ведущего.*/
    SPI_SLAVE_MODE   =   (0 << MSTR)  /*!< Режим ведомого (experimental).*/
} SpiMasterSlaveMode_t;

/*! \brief Тип данных, используемый для выбора тактовой скорости соединения.*/
typedef enum {
    SPI_SCK_F_DIV_4   =   (0 << SPR1) | (0 << SPR0), /*!< F_CPU/4.*/
    SPI_SCK_F_DIV_16  =   (0 << SPR1) | (1 << SPR0), /*!< F_CPU/16.*/
    SPI_SCK_F_DIV_64  =   (1 << SPR1) | (0 << SPR0), /*!< F_CPU/64.*/
    SPI_SCK_F_DIV_128 =   (1 << SPR1) | (1 << SPR0)  /*!< F_CPU/128.*/
} SpiClock_t;

/*! \brief Тип данных, используемый для выбора режима тактирования данных.*/
typedef enum {
    SPI_MODE_00       =   (0 << CPOL) | (0 << CPHA), /*!< Исходный уровень - низкий, установка по заднему фронту, выборка - по переднему.*/
    SPI_MODE_01       =   (0 << CPOL) | (1 << CPHA), /*!< Исходный уровень - низкий, установка по переднему фронту, выборка - по заднему.*/
    SPI_MODE_10       =   (1 << CPOL) | (0 << CPHA), /*!< Исходный уровень - высокий, установка по заднему фронту, выборка - по переднему.*/
    SPI_MODE_11       =   (1 << CPOL) | (1 << CPHA), /*!< Исходный уровень - высокий, установка по переднему фронту, выборка - по заднему.*/
} SpiMode_t;

/*! \brief Тип данных, используемый для выбора порядка передачи данных
 * в кадре.*/
typedef enum {
    SPI_DATA_ORDER_MSB_FIRST =  (0 << DORD), /*!< Первым выходит старший бит.*/
    SPI_DATA_ORDER_LSB_FIRST =  (1 << DORD), /*!< Первым выходит младший бит.*/
} SpiDataOrder_t;

/*! \brief Структура, содержащая полную конфигурацию аппаратного модуля SPI.*/
typedef struct {
    SpiMasterSlaveMode_t  master; /*!< Ведущий-ведомый.*/
    SpiClock_t            clk;    /*!< Частота тактирования данных.*/
    SpiMode_t             mode;   /*!< Режим тактирования.*/
    SpiDataOrder_t        order;  /*!< Порядок передачи бит.*/
} SpiConfig_t;

/*! \brief Функция инициализации шины SPI в режиме мастера.*/
void spi_init(SpiConfig_t* config);

/*! \brief Функция включения приемопередатчика в работу.*/
static inline void spi_enable()  { SPCR |=  (1 << SPE); }

/*! \brief Функция отключения приемопередатчика.*/
static inline void spi_disable() { SPCR &= ~(1 << SPE); }

/*! \brief Функция передачи байта данных outData.
 *
 * Использует блокирующее ожидание окончания передачи и возвращает принятый
 * обратно по ножке MOSI байт.
 *
 * \return Возвращает принятый в момент передачи байт.*/
unsigned char spi_write_blocking(char outData);

#endif /* SPI_H_ */
