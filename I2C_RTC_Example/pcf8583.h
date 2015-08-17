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

/* Адрес микросхемы PCF8583 на шине I2C */
#define PCF8583_ADDR  0b10100000

typedef enum {
	PCF8583_SUN,
	PCF8583_MON,
	PCF8583_TUE,
	PCF8583_WEN,
	PCF8583_THU,
	PCF8583_FRI,
	PCF8583_SAT
} pcf8583_dow_t;

/* Глобальные переменные, хранящие текущее время и дату */
typedef struct {
	unsigned char msec;
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char am_pm;
	unsigned char date;
	unsigned char month;
	unsigned char year;
	pcf8583_dow_t dow;
} pcf8583_time_t;

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

typedef enum {
	PCF8583_FORMAT_24H   = 0 << PCF8583_FORMAT_POS,
	PCF8583_FORMAT_12H   = 1 << PCF8583_FORMAT_POS
} pcf8583_format_t;

#define PCF8583_FUNCTION_POS     4

typedef enum {
	PCF8583_FUNCTION_32K       = 0 << PCF8583_FUNCTION_POS,
	PCF8583_FUNCTION_50HZ      = 1 << PCF8583_FUNCTION_POS,
	PCF8583_FUNCTION_COUNTER   = 2 << PCF8583_FUNCTION_POS,
	PCF8583_FUNCTION_TEST_MODE = 3 << PCF8583_FUNCTION_POS,
} pcf8583_function_t;

#define PCF8583_TIMER_FLAG      (1 << 0)
#define PCF8583_ALARM_FLAG      (1 << 1)
#define PCF8583_ALARM_ENABLE    (1 << 2)
#define PCF8583_MASK_FLAG       (1 << 3)
#define PCF8583_FUNCTION_MASK   (3 << 4)
#define PCF8583_HOLD_COUNT      (1 << 6)
#define PCF8583_STOP_COUNT      (1 << 7)

i2c_status_t pcf8583_set_function(pcf8583_function_t* function);
i2c_status_t pcf8583_set_format(pcf8583_format_t* format);
i2c_status_t pcf8583_read_time(pcf8583_time_t* t);
i2c_status_t pcf8583_write_time(const pcf8583_time_t* t);
i2c_status_t pcf8583_read_multiple(const uint8_t addr, uint8_t* data, const size_t len);
i2c_status_t pcf8583_write_multiple(const uint8_t addr, uint8_t* data, const size_t len);
i2c_status_t pcf8583_read(const uint8_t addr, uint8_t* data);
i2c_status_t pcf8583_write(const uint8_t addr, const uint8_t data);

#endif /* PCF8583_H_ */