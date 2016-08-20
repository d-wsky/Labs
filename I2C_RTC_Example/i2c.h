/*
 * i2c.h
 *
 * Created: 12-Aug-15 22:52:04
 *  Author: ����� �������������
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdlib.h>

/*! \brief ������� ������ ��� ��������� �������� ����������.*/
#define TWBR_FROM_HZ(x)  ( (F_CPU - 16UL * x) / (2UL * x * 1) + 1)

/*! \brief ���� ��� ������ ������������ ��� ��������������
�������� ���� �� ���������� ��������.*/
typedef enum {
    I2C_STATUS_OK,    /*!< �������� ���������� �������.*/
    I2C_STATUS_ERROR  /*!< ���������� ������� ���� ���������.*/
} i2c_status_t;

/*! \brief � ������� ����� ���� ����� ������� ����� ������� �����
� ������� �������������.*/
typedef enum {
    I2C_ACK, /*!< ��������� ���� ������������� (\a ACK)
                  ����� ���������� ������.*/
    I2C_NACK /*!< ���������� ���� ������������� (\a NACK)
                  ����� ���������� ������.*/
} i2c_ack_t;

/*! \brief ������� ������ ��� ���������� ������ �������� ������
�������, ����������� ������������ � ���� I2C � �������� ������������
������ � ������ ������������� �������.

\param x - ����� �� �������� ������� ��������, ������������ ������.

������ �������������:

\code
i2c_status_t write_func(uint8_t some_addr, uint8_t some_data) {
    I2C_TRY_ACTION(i2c_start());
    I2C_TRY_ACTION(i2c_write_addr(some_addr));
    I2C_TRY_ACTION(i2c_write_byte(some_data));
    I2C_TRY_ACTION(i2c_stop());
    return I2C_STATUS_OK;
}
\endcode
*/
#define I2C_TRY_ACTION(x)                    \
    do { if (x == I2C_STATUS_ERROR)          \
         return I2C_STATUS_ERROR; } while (0)

/*! \brief ������������� ����������� ����������������� I2C
���������������� �� ������ � ������ 400 ��� � ���������
��� � ������.*/
void i2c_init();

/*! \brief ��������� ��������� \a START �� ���� I2C.

\return I2C_STATUS_OK - ��� ������, I2C_STATUS_ERROR - ��������� ���������
��������� �������.*/
i2c_status_t i2c_start();

/*! \brief ��������� ��������� \a REPEAT \a START �� ���� I2C.

\return I2C_STATUS_OK - ��� ������, I2C_STATUS_ERROR - ��������� ���������
��������� �������.*/
i2c_status_t i2c_repeat_start(void);

/*! \brief ��������� ��������� \a STOP �� ���� I2C.

\return ������ I2C_STATUS_OK.*/
i2c_status_t i2c_stop();

/*! \brief �������� ������ � ������� ������/������ �� ���� I2C.

\param addr - ������������ �����.

\return I2C_STATUS_OK - ��� ������, I2C_STATUS_ERROR - �������� ������
��������� �������.*/
i2c_status_t i2c_write_addr(unsigned char addr);

/*! \brief �������� ����� ������ �� ���� I2C � ����������� ������.

\param data - ������������ ������.

\return I2C_STATUS_OK - ��� ������, I2C_STATUS_ERROR - ��������� ���������
��������� ������� (�������� ����������� ����� #I2C_MAS_TX_DATA_ACK).*/
i2c_status_t i2c_write_byte(unsigned char data);

/*! \brief ������ ����� ������ � ���� I2C.

\param data - ��������� �� ����������, ���� ����� �������� �������� ������.
\param ack - �������� ���������� ���������, ������������� �� ����
������������� (ACK) ����� ���������. � ������, ���� �������� ����������
� #I2C_MAS_RX_DATA_ACK, ��������, ��� ����� ����� ������������ � �� ����
������ ��������� ������. � ������, ���� �������� ���������� �
#I2C_MAS_RX_DATA_NACK ����������� ������ �� ����� � ����� ����� ��������
����� slave-���������� ������.

\return I2C_STATUS_OK - ��� ������, I2C_STATUS_ERROR - �������� ������
��������� �������.*/
i2c_status_t i2c_read_byte(unsigned char* data, i2c_ack_t ack);

/*! \brief ������� ������ ���������� ���� ������.

\param data - ��������� �� ������ ������� ������, ���� ����� ������� ���������
������.
\param len - ���������� ���� ������������ �� ����.

\return ���������� ����� ���������� ����

������ ������� ���������� ACK �� ��� ����������� ����� � ���������� NACK
�� ��������� �������� ����, �������� ����� �����.*/
size_t i2c_read_multiple(unsigned char* data, size_t len);

/*! \brief ������� ������ ���������� ���� ������.

\param data - ��������� �� ������ ������� ������, ������ ������� ����� �� ������.
\param len - ���������� ���� ������������ � ����.

\return ���������� ����� ������������ ����

� ������ ���������� ���� ������������� �� �������� ����������, ������
������� ���� ��������.*/
size_t i2c_write_multiple(unsigned char* data, const size_t len);

#endif /* I2C_H_ */
