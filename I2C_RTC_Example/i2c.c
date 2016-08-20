/*
 * i2c.c
 *
 * Created: 12-Aug-15 22:52:36
 *  Author: ����� �������������
 */ 

#include "i2c.h"
#include <avr/io.h>

/*! \brief ������ ��������� ��������� TWSR ����� ���������
�������� ���� I2C � ��������� \a START.*/
#define     I2C_START                    0x08
/*! \brief ������ ��������� ��������� TWSR ����� ���������
�������� ���� I2C � ��������� ���������� \a START.*/
#define     I2C_REPEAT_START            0x10
/*! \brief ������ ��������� ��������� TWSR ����� ���������
������ ������� ����� ������ �� ������ �� ���� I2C.*/
#define  I2C_MAS_TX_SLA_ACK            0x18
/*! \brief ������ ��������� ��������� TWSR ����� ���������
������ ������� ����� ������ �� ������ �� ���� I2C.*/
#define  I2C_MAS_RX_SLA_ACK            0x40
/*! \brief ������ ��������� ��������� TWSR ����� ��������
�������� ����� ������ �� ������ �� ���� I2C.*/
#define  I2C_MAS_TX_DATA_ACK        0x28
/*! \brief ������ ��������� ��������� TWSR ����� ���������
������ ����� ������ �� ���� I2C � ���������� \a ACK.*/
#define  I2C_MAS_RX_DATA_ACK        0x50
/*! \brief ������ ��������� ��������� TWSR ����� ���������
������ ����� ������ �� ���� I2C � ���������� \a NACK.*/
#define  I2C_MAS_RX_DATA_NACK        0x58

/* ����� ��� ����������� �������� ������� */
#define STATUS_MASK            0xF8
/* ������� �������� ������� ������ i2c */
#define STATUS_IS(x)         ((TWSR & STATUS_MASK) == x)
/* ���������� ������, ���� ������� �������� �������� ������� �� ������ */
#define IS_READ_ADDRESS(x)    (x & 1)

/* ������������� ����������� ����������������� I2C.*/
void i2c_init(unsigned long freq) {
    TWBR  = TWBR_FROM_HZ(freq);
    TWSR &=~((1 << TWPS1) | (1 << TWPS0));

    // ��������� ������
    TWCR |= (1 << TWEN);
}

/* ���������� ����������������� ���� I2C.*/
void i2c_close() {
    TWCR &= ~(1 << TWEN);
}

/* ��������� ��������� \a START �� ���� I2C.*/
i2c_status_t i2c_start() {
    // ��������� ���������� �����
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

    // �������� ��������� ���������
    while (!(TWCR & (1 << TWINT)));

    // �������� ������������ ��������� ����
    if (STATUS_IS(I2C_START))
        return I2C_STATUS_OK;
    else
        return I2C_STATUS_ERROR;
}

/* ��������� ��������� \a REPEAT \a START �� ���� I2C.*/
i2c_status_t i2c_repeat_start(void) {
    // �������� ��������� �����
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    
    // �������� ��������� ���������
    while (!(TWCR & (1 << TWINT)));
    
    if (STATUS_IS(I2C_REPEAT_START))
        return I2C_STATUS_OK;
    else
        return I2C_STATUS_ERROR;
}

/* ��������� ��������� \a STOP �� ���� I2C.*/
i2c_status_t i2c_stop() {
    // �������� ��������� ����
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    // �������� ��������� ���������
    while (!(TWCR & (1 << TWSTO)));
    
    return I2C_STATUS_OK;
}

/* �������� ������ � ������� ������/������ �� ���� I2C.*/
i2c_status_t i2c_write_addr(unsigned char addr) {
    unsigned char expected_status;
    
    if (IS_READ_ADDRESS(addr))
        expected_status = I2C_MAS_RX_SLA_ACK;
    else
        expected_status = I2C_MAS_TX_SLA_ACK;
    
    // ��������� ������ � ������ ��������
    TWDR = addr;
    TWCR = (1 << TWINT) | (1 << TWEN);

    // �������� ��������� ��������
    while (!(TWCR & (1 << TWINT)));
    
    // �������� ���������
    if (STATUS_IS(expected_status))
        return I2C_STATUS_OK;
    else
        return I2C_STATUS_ERROR;
}

/* �������� ����� ������ �� ���� I2C � ����������� ������.*/
i2c_status_t i2c_write_byte(unsigned char data) {
    // ���������� ������
    TWDR = data;    

    // ��������� �����������
    TWCR = (1 << TWEN) | (1 << TWINT);

    // �������� ���������� ��������
    while(!(TWCR & (1 << TWINT)));

    if (STATUS_IS(I2C_MAS_TX_DATA_ACK))
        return I2C_STATUS_OK;
    else
        return I2C_STATUS_ERROR;
}

/* ������ ����� ������ � ���� I2C � �������������� � ���.*/
i2c_status_t i2c_read_byte(unsigned char* data, i2c_ack_t ack) {
    char expected_status;
    
    //Set up ACK
    if (ack == I2C_ACK) {
        expected_status = I2C_MAS_RX_DATA_ACK;
        // �������� ACK ������ �����
        TWCR |= (1 << TWEA);
    }
    else {
        expected_status = I2C_MAS_RX_DATA_NACK;
        // �������� NACK ����� ������ �����
        TWCR &= ~(1 << TWEA);
    }
    
    // ���������� ������ �������� TWINT
    TWCR |= (1 << TWINT);

    // �������� ���������� ������
    while (!(TWCR & (1<<TWINT)));

    if (STATUS_IS(expected_status)) {
        // ���������� ������ �����
        *data = TWDR;
        return I2C_STATUS_OK;
    }
    else {
        return I2C_STATUS_ERROR;
    }
}

/* ������� ������ ���������� ���� ������.*/
size_t i2c_read_multiple(unsigned char* data, const size_t len) {
    size_t rx_count = 0;
    for (size_t i = 1; i < len; i++) {
        if (i2c_read_byte(data++, I2C_ACK) == I2C_STATUS_ERROR) {
            return rx_count;
        }
        rx_count++;
    }
    if (i2c_read_byte(data, I2C_NACK) == I2C_STATUS_OK) {
        rx_count++;
    }
    return rx_count;
}

/* ������� ������ ���������� ���� ������.*/
size_t i2c_write_multiple(unsigned char* data, const size_t len) {
    size_t tx_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (i2c_write_byte(*data++) == I2C_STATUS_ERROR) {
            return tx_count;
        }
        tx_count++;
    }
    return tx_count;
}
