/*
 * pcf8583.h
 *
 * Created: 14-Aug-15 00:18:30
 *  Author: ����� �������������
 */ 


#ifndef PCF8583_H_
#define PCF8583_H_

#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>

/*! \brief ����� ���������� PCF8583 �� ���� I2C */
#define PCF8583_ADDR  0b10100000

//------------------------------------
// ������� ����������/���������
//------------------------------------

/*! \brief �� ������ �������� ����������� �������������
������ ������������� ������� � ����������� 50%, ����
�������� ������ ����������.*/
#define PCF8583_TIMER_FLAG      (1 << 0)
/*! \brief �� ������ �������� ����������� �������������
������ ������������� ������� � ����������� 50%, ����
�������� ������ ����������.*/
#define PCF8583_ALARM_FLAG      (1 << 1)
/*! \brief ���� ��������� ����������.*/
#define PCF8583_ALARM_ENABLE    (1 << 2)
/*! \brief ���� ������������ ���� � ������.*/
#define PCF8583_MASK_FLAG       (1 << 3)
/*! \brief ������� ��� ���� ������ ������.*/
#define PCF8583_MODE_POS         4
/*! \brief ����� ������ ������ ������ ����������.*/
#define PCF8583_MODE_MASK       (3 << 4)
/*! \brief ������ �������� ������� ���������.

���� ���� ��� ���������� � 1, �� ���������� ������.*/
#define PCF8583_HOLD_COUNT      (1 << 6)
/*! \brief ���� ��������� �����.*/
#define PCF8583_STOP_COUNT      (1 << 7)

//------------------------------------
// �������� �������
//------------------------------------

/* ��������������� ������� ��� ������ � ������.*/
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
// �������� ���������� �����������
//------------------------------------
#define PCF8583_ALARM_FUNC_POS       4
#define PCF8583_TIMER_FUNC_POS       0

//------------------------------------
// ��������������� ���� ������
//------------------------------------

/*! \brief �������� ������ ������������� �����.*/
typedef enum {
    PCF8583_FORMAT_24H   = 0 << PCF8583_FORMAT_POS, /*!< 24-������� ������.*/
    PCF8583_FORMAT_12H   = 1 << PCF8583_FORMAT_POS  /*!< 12-������� ������.*/
} pcf8583_format_t;

/*! \brief �������� ����� ������ �����.*/
typedef enum {
    PCF8583_MODE_32K       = 0 << PCF8583_MODE_POS, /*!< ���� �� ������ 32768 ��.*/
    PCF8583_MODE_50HZ      = 1 << PCF8583_MODE_POS, /*!< ���� �� ���� 50 ��.*/
    PCF8583_MODE_COUNTER   = 2 << PCF8583_MODE_POS, /*!< �������.*/
    PCF8583_MODE_TEST      = 3 << PCF8583_MODE_POS, /*!< �������� �����.*/
} pcf8583_mode_t;

/*! \brief ������ ����� ������������ ����������.*/
typedef enum {
    PCF8583_ALARM_FUNC_OFF     = 0 << PCF8583_ALARM_FUNC_POS, /*!< ��������� ��������.*/
    PCF8583_ALARM_FUNC_DAILY   = 1 << PCF8583_ALARM_FUNC_POS, /*!< ��������� ����������� ���������.*/
    PCF8583_ALARM_FUNC_WEEKDAY = 2 << PCF8583_ALARM_FUNC_POS, /*!< ��������� �� ���� ������.*/
    PCF8583_ALARM_FUNC_DATED   = 3 << PCF8583_ALARM_FUNC_POS, /*!< ��������� ����������� � �������� ����.*/
} pcf8583_alarm_func_t;

/*! \brief ������ �������� ����� �������.*/
typedef enum {
    PCF8583_TIMER_FUNC_OFF     = 0 << PCF8583_TIMER_FUNC_POS, /*!< ������ ��������.*/
    PCF8583_TIMER_FUNC_HSEC    = 1 << PCF8583_TIMER_FUNC_POS, /*!< ������ ����������� ����� ���� �������.*/
    PCF8583_TIMER_FUNC_SEC     = 2 << PCF8583_TIMER_FUNC_POS, /*!< ������ ����������� �������.*/
    PCF8583_TIMER_FUNC_MIN     = 3 << PCF8583_TIMER_FUNC_POS, /*!< ������ ����������� ������.*/
    PCF8583_TIMER_FUNC_HOUR    = 4 << PCF8583_TIMER_FUNC_POS, /*!< ������ ����������� ����.*/
    PCF8583_TIMER_FUNC_DAY     = 5 << PCF8583_TIMER_FUNC_POS, /*!< ������ ����������� ���.*/
} pcf8583_timer_func_t;

/*! \brief ��� ������, ������� ����� ������������ ��� �����������
��� ������.*/
typedef enum {
    PCF8583_SUN = 0, /*!< �����������.*/
    PCF8583_MON,     /*!< �����������.*/
    PCF8583_TUE,     /*!< �������.*/
    PCF8583_WEN,     /*!< �����.*/
    PCF8583_THU,     /*!< �������.*/
    PCF8583_FRI,     /*!< �������.*/
    PCF8583_SAT      /*!< �������.*/
} pcf8583_dow_t;

/*! \brief ��� ������, ������������ ��� ������ � �������� �������
� ���� ����� �� � ����������� �����.

��� ���� ����� �������������� ������������� BCD, ����� �������,
��������� ��������.*/
typedef struct {
    unsigned char hsec;  /*!< ����� ���� ������� (0x00 - 0x99).*/
    unsigned char sec;   /*!< ������� (0x00 - 0x59).*/
    unsigned char min;   /*!< ������ (0x00 - 0x59).*/
    unsigned char hour;  /*!< ���� (0x00 - 0x12 ��� 0x24).*/
    unsigned char am_pm; /*!< ������������� ��-����� �������. 0 - AM; 1 - PM.*/
    unsigned char date;  /*!< ���� (0x01 - 0x31).
                              
                              �������� ������� �� ������ � ����. ��� �������
                              �������� ���� �������� - 0x29.*/
    unsigned char month; /*!< ����� (0x01 - 0x12).*/
    unsigned char year;  /*!< ��� (0x00 - 0x03).
    
                              ���������� ��� 0x00.*/
    pcf8583_dow_t dow;   /*!< ���� ������.*/
} pcf8583_time_t;


//------------------------------------
// ��������� �������
//------------------------------------

/*! \brief ������� ������� �����. ����������� ����������.*/
i2c_status_t pcf8583_start();

/*! \brief ������� ��������� �����. ����������� ����������.*/
i2c_status_t pcf8583_stop();

/*! \brief ����� ������ ������ �����. ����������� ����������.*/
i2c_status_t pcf8583_set_mode(pcf8583_mode_t mode);

/*! \brief ����� ������� ������������� ������� (12-������� ��� 24-�������).
����������� ����������.*/
i2c_status_t pcf8583_set_format(pcf8583_format_t format);

/*! \brief ������� ��������� ������ ����������. ����������� ����������.

\param func - ����� ������ ����������.*/
i2c_status_t pcf8583_set_alarm_func(const pcf8583_alarm_func_t func);

/*! \brief ������� ��������� ������� ����������. ����������� ����������.

\param alarm_time - ����� ������������. ���� ��������������� ���������� �����,
�� ���� ���� � ��� ������ ����� ���� ���������������.
\param dow - ��� ��������� ���������� �� ���� ������ � ���� �������� ����������
�������� ���, ������������ ����� ��������� �.
\param irq_enable - ���������� ���������� ��� ������������ ����������. ����
���������, �� �� ���� ����� ������ �������� AF �� �������� �������.*/
i2c_status_t pcf8583_set_alarm(const pcf8583_time_t* alarm_time,
                               const pcf8583_dow_t dow,
                               bool irq_enable);

/*! \brief ������� ��������� ������ �������. ����������� ����������.

\param func - ����� ������ �������.*/
i2c_status_t pcf8583_set_timer_func(const pcf8583_timer_func_t func);

/*! \brief ������� ��������� �������. ����������� ����������.

\param timer_timeout - ����� � ������� BCD �� 0x00 �� 0x99, ������� �����
�������� ��������� �������� �������. �������� ����� ������� �������� � �������
\a pcf8583_set_timer_func, � ����� �� ������������ �� 0x00 � 0x99 ���������
����������.
\param irq_enable - ���������� ���������� ��� ������������ ����������. ����
���������, �� �� ���� ����� ������ �������� TF �� �������� �������.*/
i2c_status_t pcf8583_set_timer(const uint8_t timer_timeout,
                               bool irq_enable);

/*! \brief ������ ������� �� �����.

\param t - � ��� ���������� ����� ��������� ��������� ������, ����
��� ������ �����.*/
i2c_status_t pcf8583_read_time(pcf8583_time_t* t);

/*! \brief ��������� ������� �� �����.

\param t - ��������� �� ���������� �������� �������� �����.*/
i2c_status_t pcf8583_write_time(const pcf8583_time_t* t);

/*! \brief ������ ��������� �������� �� �����. ����������� ����������.

\param cnt - � ��� ���������� ����� ��������� ��������� ������, ����
��� ������ �����.*/
i2c_status_t pcf8583_read_counter(uint32_t* cnt);

/*! \brief ��������� ������ �������� ��������, ��� ������ �����
� ������ ��������. ����������� ����������.

\param cnt - ��������� �� ���������� �������� �������� �����.*/
i2c_status_t pcf8583_write_counter(const uint32_t* cnt);

/*! \brief ������ ������ �� ������������ ������ ������ �����.

\param addr - ����� ������.
\param data - ��������� �� ������� ������, ���� ����� ��������� ���������
������.

\warning ������ 0-0xF ��������������� ��� ���������� ������ � ��������.*/
i2c_status_t pcf8583_read(const uint8_t addr, uint8_t* data);

/*! \brief ������ ������ �� ������������ ������������������ �����
������ �����.

\param addr - ����� ��������� ������.
\param data - ��������� �� ������� ������, ���� ����� ��������� ���������
������.
\param len - ���������� ����� ��� ������ (�������� ������ 0x00-0xFF).

\warning ������ 0-0xF ��������������� ��� ���������� ������ � ��������.*/
i2c_status_t pcf8583_read_multiple(const uint8_t addr, uint8_t* data, const size_t len);

/*! \brief ������ ������ � ������������ ������ ������ �����.

\param addr - ����� ������.
\param data - ������, ������� ���������� ��������.

\warning ������ 0-0xF ��������������� ��� ���������� ������ � ��������.*/
i2c_status_t pcf8583_write(const uint8_t addr, const uint8_t data);

/*! \brief ������ ������ � ������������ ������������������ ����� ������ �����.

\param addr - ����� ������.
\param data - ��������� �� ������ ������� ������, ������� ���������� ��������.
\param len - ������ ������� ��� ������.

\warning ������ 0-0xF ��������������� ��� ���������� ������ � ��������.*/
i2c_status_t pcf8583_write_multiple(const uint8_t addr, uint8_t* data, const size_t len);

#endif /* PCF8583_H_ */
