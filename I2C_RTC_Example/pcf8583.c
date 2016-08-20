/*
 * pcf8583.c
 *
 * Created: 14-Aug-15 00:21:52
 *  Author: Денис Васильковский
 */ 

#include "pcf8583.h"

#define PCF8583_ADDR_READ        (PCF8583_ADDR | 1)
#define PCF8583_ADDR_WRITE       (PCF8583_ADDR | 0)

#define PCF8583_TIME_ADDR              1
#define PCF8583_HSEC_REG               0
#define PCF8583_SEC_REG                1
#define PCF8583_MIN_REG                2
#define PCF8583_HOURS_REG              3
#define PCF8583_AM_PM_REG              3
#define PCF8583_DAYS_REG               4
#define PCF8583_YEARS_REG              4
#define PCF8583_MONTHS_REG             5
#define PCF8583_DOW_REG                5
#define PCF8583_PACKED_TIME_LEN        6

/* Считывание текущего времени из микросхемы I2C */
i2c_status_t pcf8583_read_multiple(const uint8_t addr, uint8_t* data, const size_t len) {
    /* начало передачи */
    I2C_TRY_ACTION(i2c_start());
    /* выбор микросхемы и регистра */
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_WRITE));
    I2C_TRY_ACTION(i2c_write_byte(addr));
    /* чтение данных */
    I2C_TRY_ACTION(i2c_repeat_start());
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_READ));
    I2C_TRY_ACTION(i2c_read_multiple(data, len));
    /* завершение передачи */
    I2C_TRY_ACTION(i2c_stop());
    return I2C_STATUS_OK;
}

/* склейка трех аргументов */
#define CONCAT_3(p1, p2, p3)           p1##p2##p3
/* распаковка данных из часов в структуру */
#define UNPACK_FIELD(from, field)    ((from[CONCAT_3(PCF8583_, field, _REG)] & CONCAT_3(PCF8583_, field, _MASK)) >> CONCAT_3(PCF8583_, field, _POS))
/* упаковка данных из структуры в регистр часов */
#define PACK_FIELD(to, from, field)    do \
                                       {  to[CONCAT_3(PCF8583_, field, _REG)] |= \
                                         ((from << CONCAT_3(PCF8583_, field, _POS)) & CONCAT_3(PCF8583_, field, _MASK)); \
                                       } while (0)
  
/* чтение времени */
i2c_status_t pcf8583_read_time(pcf8583_time_t* t) {
    /* выделение памяти */
    uint8_t packed_time[PCF8583_PACKED_TIME_LEN];
    
    /* чтение данных */
    i2c_status_t res = pcf8583_read_multiple(PCF8583_TIME_ADDR, packed_time, sizeof(packed_time));
    if (res != I2C_STATUS_OK) {
        return res;
    }
    
    /* преобразование под выходной формат */
    t->hsec  = UNPACK_FIELD(packed_time, HSEC);
    t->sec   = UNPACK_FIELD(packed_time, SEC);
    t->min   = UNPACK_FIELD(packed_time, MIN);
    t->hour  = UNPACK_FIELD(packed_time, HOURS);
    t->date  = UNPACK_FIELD(packed_time, DAYS);
    t->month = UNPACK_FIELD(packed_time, MONTHS);
    t->year  = UNPACK_FIELD(packed_time, YEARS);
    t->dow   = UNPACK_FIELD(packed_time, DOW);
    t->am_pm = UNPACK_FIELD(packed_time, AM_PM);
    
    return res;
}

/* запись последовательности байт */
i2c_status_t pcf8583_write_multiple(const uint8_t addr, uint8_t* data, const size_t len) {
    /* начало передачи */
    I2C_TRY_ACTION(i2c_start());
    /* выбор микросхемы */
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_WRITE));
    /* выбор начального регистра */
    I2C_TRY_ACTION(i2c_write_byte(addr));
    /* передача данных */
    I2C_TRY_ACTION(i2c_write_multiple(data, len));
    /* завершение передачи */
    I2C_TRY_ACTION(i2c_stop());
    return I2C_STATUS_OK;
}

/* запись нового времени в часы */
i2c_status_t pcf8583_write_time(const pcf8583_time_t* t) {
    /* выделение памяти */
    uint8_t packed_time[PCF8583_PACKED_TIME_LEN] = {0};
    
    /* преобразование под выходной формат */
    PACK_FIELD(packed_time, t->hsec,  HSEC);
    PACK_FIELD(packed_time, t->sec,   SEC);
    PACK_FIELD(packed_time, t->min,   MIN);
    PACK_FIELD(packed_time, t->hour,  HOURS);
    PACK_FIELD(packed_time, t->date,  DAYS);
    PACK_FIELD(packed_time, t->month, MONTHS);
    PACK_FIELD(packed_time, t->year,  YEARS);
    PACK_FIELD(packed_time, t->dow,   DOW);
    PACK_FIELD(packed_time, t->am_pm, AM_PM);
    
    /* запись данных в микросхему */
    i2c_status_t res = pcf8583_write_multiple(PCF8583_TIME_ADDR, packed_time, sizeof(packed_time));
    if (res != I2C_STATUS_OK) {
        return res;
    }
    
    return res;
}

/* запись байта в часы */
i2c_status_t pcf8583_write(const uint8_t addr, const uint8_t data) {
    /* начало передачи */
    I2C_TRY_ACTION(i2c_start());
    /* выбор микросхемы */
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_WRITE));
    /* выбор начального регистра */
    I2C_TRY_ACTION(i2c_write_byte(addr));
    /* передача данных */
    I2C_TRY_ACTION(i2c_write_byte(data));
    /* завершение передачи */
    I2C_TRY_ACTION(i2c_stop());
    return I2C_STATUS_OK;
}

/* чтение байта из часов */
i2c_status_t pcf8583_read(const uint8_t addr, uint8_t* data) {
    I2C_TRY_ACTION(i2c_start());
    /* выбор регистра чтения */
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_WRITE));
    I2C_TRY_ACTION(i2c_write_byte(addr));
    /* операция чтения */
    I2C_TRY_ACTION(i2c_repeat_start());
    I2C_TRY_ACTION(i2c_write_addr(PCF8583_ADDR_READ));
    I2C_TRY_ACTION(i2c_read_byte(data, I2C_NACK));
    I2C_TRY_ACTION(i2c_stop());
    return I2C_STATUS_OK;
}
