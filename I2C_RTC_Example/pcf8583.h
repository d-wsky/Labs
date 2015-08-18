/*
 * pcf8583.h
 *
 * Created: 14-Aug-15 00:18:30
 *  Author: Денис Васильковский
 */ 


#ifndef PCF8583_H_
#define PCF8583_H_

#include "i2c.h"
#include <stdint.h>

/*! \brief Адрес микросхемы PCF8583 на шине I2C */
#define PCF8583_ADDR  0b10100000

/*! \brief Тип данных, который можно использовать для определения
дня недели.*/
typedef enum {
	PCF8583_SUN = 0, /*!< Воскресенье.*/
	PCF8583_MON,     /*!< Понедельник.*/
	PCF8583_TUE,     /*!< Вторник.*/
	PCF8583_WEN,     /*!< Среда.*/
	PCF8583_THU,     /*!< Четверг.*/
	PCF8583_FRI,     /*!< Пятница.*/
	PCF8583_SAT      /*!< Суббота.*/
} pcf8583_dow_t;

/*! \brief Тип данных, используемый для приема и передачи времени
и даты между МК и микросхемой часов.

Для всех полей предполагается представление BCD, кроме случаев,
указанных отдельно.*/
typedef struct {
	unsigned char msec;  /*!< Миллисекунды (0x00 - 0x99).*/
	unsigned char sec;   /*!< Секунды (0x00 - 0x59).*/
	unsigned char min;   /*!< Минуты (0x00 - 0x59).*/
	unsigned char hour;  /*!< Часы (0x00 - 0x12 или 0x24).*/
	unsigned char am_pm; /*!< Переключатель до-после полудня. 0 - AM; 1 - PM.*/
	unsigned char date;  /*!< Дата (0x01 - 0x31).
	                          
							  Значение зависит от месяца и года. Для февраля
							  нулевого года максимум - 0x29.*/
	unsigned char month; /*!< Месяц (0x01 - 0x12).*/
	unsigned char year;  /*!< Год (0x00 - 0x03).
	
	                          Високосный год 0x00.*/
	pcf8583_dow_t dow;   /*!< День недели.*/
} pcf8583_time_t;

/* Вспомогательные макросы для работы с часами.*/
#define PCF8583_MSEC_MASK           (0xFF)
#define PCF8583_MSEC_POS             0

#define PCF8583_SEC_MASK            (0xFF)
#define PCF8583_SEC_POS              0

#define PCF8583_MIN_MASK            (0xFF)
#define PCF8583_MIN_POS              0

#define PCF8583_HOURS_MASK          (0x3F)
#define PCF8583_HOURS_POS            0

#define PCF8583_AM_PM_POS            6
#define PCF8583_AM_PM_MASK          (1 << PCF8583_AM_PM_POS)

#define PCF8583_FORMAT_POS           7

#define PCF8583_DAYS_POS            (0)
#define PCF8583_DAYS_MASK           (0x3F)

#define PCF8583_YEARS_POS            6
#define PCF8583_YEARS_MASK          (3 << PCF8583_YEARS_POS)

#define PCF8583_MONTHS_POS           0
#define PCF8583_MONTHS_MASK         (0x1F)

#define PCF8583_DOW_POS              5
#define PCF8583_DOW_MASK            (7 << PCF8583_DOW_POS)

/*! \brief Выбирает формат представления часов.*/
typedef enum {
	PCF8583_FORMAT_24H   = 0 << PCF8583_FORMAT_POS, /*!< 24-часовой формат.*/
	PCF8583_FORMAT_12H   = 1 << PCF8583_FORMAT_POS  /*!< 12-часовой формат.*/
} pcf8583_format_t;

#define PCF8583_FUNCTION_POS     4

/*! \brief Выбирает режим работы часов.*/
typedef enum {
	PCF8583_FUNCTION_32K       = 0 << PCF8583_FUNCTION_POS, /*!< Часы от кварца 32768 Гц.*/
	PCF8583_FUNCTION_50HZ      = 1 << PCF8583_FUNCTION_POS, /*!< Часы от сети 50 Гц.*/
	PCF8583_FUNCTION_COUNTER   = 2 << PCF8583_FUNCTION_POS, /*!< Счетчик.*/
	PCF8583_FUNCTION_TEST_MODE = 3 << PCF8583_FUNCTION_POS, /*!< Тестовый режим.*/
} pcf8583_function_t;

/*! \brief На данном регистре формируется прямоугольный
сигнал длительностью секунда и заполнением 50%, если
отключен сигнал будильника.*/
#define PCF8583_TIMER_FLAG      (1 << 0)
/*! \brief На данном регистре формируется прямоугольный
сигнал длительностью секунда и заполнением 50%, если
отключен сигнал будильника.*/
#define PCF8583_ALARM_FLAG      (1 << 1)
/*! \brief Флаг включения будильника.*/
#define PCF8583_ALARM_ENABLE    (1 << 2)
/*! \brief Флаг маскирования даты и месяца.*/
#define PCF8583_MASK_FLAG       (1 << 3)
/*! \brief Маска выбора режима работы микросхемы.*/
#define PCF8583_FUNCTION_MASK   (3 << 4)
/*! \brief Захват значения счетных регистров.

Если этот бит установлен в 1, то происходит захват.*/
#define PCF8583_HOLD_COUNT      (1 << 6)
/*! \brief Флаг остановки счета.*/
#define PCF8583_STOP_COUNT      (1 << 7)

/*! \brief Функция запуска часов. Отсутствует реализация.*/
i2c_status_t pcf8583_start();

/*! \brief Функция остановки часов. Отсутствует реализация.*/
i2c_status_t pcf8583_stop();

/*! \brief Выбор режима работы часов. Отсутствует реализация.*/
i2c_status_t pcf8583_set_function(pcf8583_function_t* function);

/*! \brief Выбор формата представления времени (12-часовой или 24-часовой).
Отсутствует реализация.*/
i2c_status_t pcf8583_set_format(pcf8583_format_t* format);

/*! \brief Чтение времени из часов.

\param t - в эту переменную будет возвращен результат чтения, если
оно прошло верно.*/
i2c_status_t pcf8583_read_time(pcf8583_time_t* t);

/*! \brief Установка времени на часах.

\param t - указатель на переменную хранящую желаемое время.*/
i2c_status_t pcf8583_write_time(const pcf8583_time_t* t);

/*! \brief Чтение данных из произвольной ячейки памяти часов.

\param addr - номер ячейки.
\param data - указатель на область памяти, куда будет возвращен результат
чтения.

\warning Адреса 0-0xF зарезервированы для управления часами и временем.*/
i2c_status_t pcf8583_read(const uint8_t addr, uint8_t* data);

/*! \brief Чтение данных из произвольной последовательности ячеек
памяти часов.

\param addr - номер стартовой ячейки.
\param data - указатель на область памяти, куда будет возвращен результат
чтения.
\param len - количество ячеек для чтения (доступны адреса 0x00-0xFF).

\warning Адреса 0-0xF зарезервированы для управления часами и временем.*/
i2c_status_t pcf8583_read_multiple(const uint8_t addr, uint8_t* data, const size_t len);

/*! \brief Запись данных в произвольную ячейку памяти часов.

\param addr - номер ячейки.
\param data - данные, которые необходимо записать.

\warning Адреса 0-0xF зарезервированы для управления часами и временем.*/
i2c_status_t pcf8583_write(const uint8_t addr, const uint8_t data);

/*! \brief Запись данных в произвольную последовательность ячеек памяти часов.

\param addr - номер ячейки.
\param data - указатель на начало участка памяти, который необходимо записать.
\param len - размер области для записи.

\warning Адреса 0-0xF зарезервированы для управления часами и временем.*/
i2c_status_t pcf8583_write_multiple(const uint8_t addr, uint8_t* data, const size_t len);

#endif /* PCF8583_H_ */
