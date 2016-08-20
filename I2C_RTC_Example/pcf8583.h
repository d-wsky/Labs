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
#include <stdbool.h>

/*! \brief Адрес микросхемы PCF8583 на шине I2C */
#define PCF8583_ADDR  0b10100000

//------------------------------------
// Регистр управления/состояния
//------------------------------------

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
/*! \brief Позиция для поля режима работы.*/
#define PCF8583_MODE_POS         4
/*! \brief Маска выбора режима работы микросхемы.*/
#define PCF8583_MODE_MASK       (3 << 4)
/*! \brief Захват значения счетных регистров.

Если этот бит установлен в 1, то происходит захват.*/
#define PCF8583_HOLD_COUNT      (1 << 6)
/*! \brief Флаг остановки счета.*/
#define PCF8583_STOP_COUNT      (1 << 7)

//------------------------------------
// Регистры времени
//------------------------------------

/* Вспомогательные макросы для работы с часами.*/
#define PCF8583_HSEC_MASK           (0xFF)
#define PCF8583_HSEC_POS             0

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

//------------------------------------
// Регистры управления будильником
//------------------------------------
#define PCF8583_ALARM_FUNC_POS       4
#define PCF8583_TIMER_FUNC_POS       0

//------------------------------------
// Вспомогательные типы данных
//------------------------------------

/*! \brief Выбирает формат представления часов.*/
typedef enum {
	PCF8583_FORMAT_24H   = 0 << PCF8583_FORMAT_POS, /*!< 24-часовой формат.*/
	PCF8583_FORMAT_12H   = 1 << PCF8583_FORMAT_POS  /*!< 12-часовой формат.*/
} pcf8583_format_t;

/*! \brief Выбирает режим работы часов.*/
typedef enum {
	PCF8583_MODE_32K       = 0 << PCF8583_MODE_POS, /*!< Часы от кварца 32768 Гц.*/
	PCF8583_MODE_50HZ      = 1 << PCF8583_MODE_POS, /*!< Часы от сети 50 Гц.*/
	PCF8583_MODE_COUNTER   = 2 << PCF8583_MODE_POS, /*!< Счетчик.*/
	PCF8583_MODE_TEST      = 3 << PCF8583_MODE_POS, /*!< Тестовый режим.*/
} pcf8583_mode_t;

/*! \brief Задает режим срабатывания будильника.*/
typedef enum {
	PCF8583_ALARM_FUNC_OFF     = 0 << PCF8583_ALARM_FUNC_POS, /*!< Будильник отключен.*/
	PCF8583_ALARM_FUNC_DAILY   = 1 << PCF8583_ALARM_FUNC_POS, /*!< Будильник срабатывает ежедневно.*/
	PCF8583_ALARM_FUNC_WEEKDAY = 2 << PCF8583_ALARM_FUNC_POS, /*!< Будильник по дням недели.*/
	PCF8583_ALARM_FUNC_DATED   = 3 << PCF8583_ALARM_FUNC_POS, /*!< Будильник срабатывает в заданную дату.*/
} pcf8583_alarm_func_t;

/*! \brief Задает скорость счета таймера.*/
typedef enum {
	PCF8583_TIMER_FUNC_OFF     = 0 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отключен.*/
	PCF8583_TIMER_FUNC_HSEC    = 1 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отсчитывает сотые доли секунды.*/
	PCF8583_TIMER_FUNC_SEC     = 2 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отсчитывает секунды.*/
	PCF8583_TIMER_FUNC_MIN     = 3 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отсчитывает минуты.*/
	PCF8583_TIMER_FUNC_HOUR    = 4 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отсчитывает часы.*/
	PCF8583_TIMER_FUNC_DAY     = 5 << PCF8583_TIMER_FUNC_POS, /*!< Таймер отсчитывает дни.*/
} pcf8583_timer_func_t;

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
	unsigned char hsec;  /*!< Сотые доли секунды (0x00 - 0x99).*/
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


//------------------------------------
// Публичные функции
//------------------------------------

/*! \brief Функция запуска часов. Отсутствует реализация.*/
i2c_status_t pcf8583_start();

/*! \brief Функция остановки часов. Отсутствует реализация.*/
i2c_status_t pcf8583_stop();

/*! \brief Выбор режима работы часов. Отсутствует реализация.*/
i2c_status_t pcf8583_set_mode(pcf8583_mode_t mode);

/*! \brief Выбор формата представления времени (12-часовой или 24-часовой).
Отсутствует реализация.*/
i2c_status_t pcf8583_set_format(pcf8583_format_t format);

/*! \brief Функция установки режима будильника. Отсутствует реализация.

\param func - режим работы будильника.*/
i2c_status_t pcf8583_set_alarm_func(const pcf8583_alarm_func_t func);

/*! \brief Функция установки времени будильника. Отсутствует реализация.

\param alarm_time - время срабатывания. Если устанавливается ежедневный режим,
то поля даты и дня недели могут быть проигнорированы.
\param dow - при настройке будильника по дням недели в этот параметр передаются
желаемые дни, объединенные через побитовое И.
\param irq_enable - разрешение прерывания при срабатывании будильника. Если
отключено, то об этом можно узнать прочитав AF из регистра статуса.*/
i2c_status_t pcf8583_set_alarm(const pcf8583_time_t* alarm_time,
                               const pcf8583_dow_t dow,
                               bool irq_enable);

/*! \brief Функция установки режима таймера. Отсутствует реализация.

\param func - режим работы таймера.*/
i2c_status_t pcf8583_set_timer_func(const pcf8583_timer_func_t func);

/*! \brief Функция установки таймера. Отсутствует реализация.

\param timer_timeout - число в формате BCD от 0x00 до 0x99, которое будет
задавать начальное значение таймера. Скорость счета таймера задается с помощью
\a pcf8583_set_timer_func, и когда он переключится из 0x00 в 0x99 возникнет
прерывание.
\param irq_enable - разрешение прерывания при срабатывании будильника. Если
отключено, то об этом можно узнать прочитав TF из регистра статуса.*/
i2c_status_t pcf8583_set_timer(const uint8_t timer_timeout,
                               bool irq_enable);

/*! \brief Чтение времени из часов.

\param t - в эту переменную будет возвращен результат чтения, если
оно прошло верно.*/
i2c_status_t pcf8583_read_time(pcf8583_time_t* t);

/*! \brief Установка времени на часах.

\param t - указатель на переменную хранящую желаемое время.*/
i2c_status_t pcf8583_write_time(const pcf8583_time_t* t);

/*! \brief Чтение показаний счетчика из часов. Отсутствует реализация.

\param cnt - в эту переменную будет возвращен результат чтения, если
оно прошло верно.*/
i2c_status_t pcf8583_read_counter(uint32_t* cnt);

/*! \brief Установка нового значения счетчика, при работе часов
в режиме счетчика. Отсутствует реализация.

\param cnt - указатель на переменную хранящую желаемое число.*/
i2c_status_t pcf8583_write_counter(const uint32_t* cnt);

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
