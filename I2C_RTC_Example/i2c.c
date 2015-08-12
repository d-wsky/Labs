/*
 * i2c.c
 *
 * Created: 12-Aug-15 22:52:36
 *  Author: Денис Васильковский
 */ 

#include "i2c.h"
#include <avr/io.h>

/*! \brief Макрос описывает состояние TWSR после успешного
перехода шины I2C в состояние \a START.*/
#define	 I2C_START				    0x08
/*! \brief Макрос описывает состояние TWSR после успешного
перехода шины I2C в состояние повторного \a START.*/
#define	 I2C_REPEAT_START			0x10
/*! \brief Макрос описывает состояние TWSR после успешной
отправки байта адреса по шине I2C в режиме мастера.*/
#define  I2C_MAS_TX_SLA_ACK			0x18
/*! \brief Макрос описывает состояние TWSR после успешного
приема байта адреса по шине I2C в режиме мастера.*/
#define  I2C_MAS_RX_SLA_ACK			0x40
/*! \brief Макрос описывает состояние TWSR после успешной
передачи байта данных по шине I2C.*/
#define  I2C_MAS_TX_DATA_ACK		0x28
/*! \brief Макрос описывает состояние TWSR после успешного
приема байта данных по шине I2C с установкой \a ACK.*/
#define  I2C_MAS_RX_DATA_ACK		0x50
/*! \brief Макрос описывает состояние TWSR после успешного
приема байта данных по шине I2C с установкой \a NACK.*/
#define  I2C_MAS_RX_DATA_NACK		0x58

#define STATUS_MASK            0xF8
#define STATUS_IS(x)         ((TWSR & STATUS_MASK) == x)
#define IS_READ_ADDRESS(x)    (x & 1)

/* Инициализация внутреннего приемопередатчика I2C.*/
void i2c_init(unsigned long freq) {
	TWBR  = TWBR_FROM_HZ(freq);
	TWSR &=~((1 << TWPS1) | (1 << TWPS0));

	// включение модуля
	TWCR |= (1 << TWEN);
}

/* Отключение приемопередатчика шины I2C.*/
void i2c_close() {
	TWCR &= ~(1 << TWEN);
}

/* Установка состояния \a START на шине I2C.*/
i2c_status_t i2c_start() {
	// Установка ссостояния старт
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);

	// Ожидание изменения состояния
	while (!(TWCR & (1 << TWINT)));

	// Проверка правильности состояния шины
	if (STATUS_IS(I2C_START))
		return I2C_STATUS_OK;
	else
		return I2C_STATUS_ERROR;
}

/* Установка состояния \a REPEAT \a START на шине I2C.*/
i2c_status_t i2c_repeat_start(void) {
	// Отправка состояния старт
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	
	// Ожидание изменения состояния
	while (!(TWCR & (1 << TWINT)));
	
	if (STATUS_IS(I2C_REPEAT_START))
		return I2C_STATUS_OK;
	else
		return I2C_STATUS_ERROR;
}

/* Установка состояния \a STOP на шине I2C.*/
i2c_status_t i2c_stop() {
	// передача состояния СТОП
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

	// Ожидание изменения состояния
	while (!(TWCR & (1 << TWSTO)));
	
	return I2C_STATUS_OK;
}

/* Передача адреса и команды чтения/записи по шине I2C.*/
i2c_status_t i2c_write_addr(unsigned char addr) {
	unsigned char expected_status;
	
	if (IS_READ_ADDRESS(addr))
		expected_status = I2C_MAS_RX_SLA_ACK;
	else
		expected_status = I2C_MAS_TX_SLA_ACK;
	
	// установка адреса и начало передачи
	TWDR = addr;
	TWCR = (1 << TWINT) | (1 << TWEN);

	// ожидание окончания передачи
	while (!(TWCR & (1 << TWINT)));
	
	// проверка состояния
	if (STATUS_IS(expected_status))
		return I2C_STATUS_OK;
	else
		return I2C_STATUS_ERROR;
}

/* Передача байта данных по шине I2C в блокирующем режиме.*/
i2c_status_t i2c_write_byte(unsigned char data) {
	// Заполнение буфера
	TWDR = data;	

	// Включение передатчика
	TWCR = (1 << TWEN) | (1 << TWINT);

	// Ожидание завершения передачи
	while(!(TWCR & (1 << TWINT)));

	if (STATUS_IS(I2C_MAS_TX_DATA_ACK))
		return I2C_STATUS_OK;
	else
		return I2C_STATUS_ERROR;
}

/* Чтение байта данных с шины I2C с подтверждением и без.*/
i2c_status_t i2c_read_byte(unsigned char* data, i2c_ack_t ack) {
	char expected_status;
	
	//Set up ACK
	if (ack == I2C_ACK) {
		expected_status = I2C_MAS_RX_DATA_ACK;
		// передача ACK приема байта
		TWCR |= (1 << TWEA);
	}
	else {
		expected_status = I2C_MAS_RX_DATA_NACK;
		// передача NACK после приема байта
		TWCR &= ~(1 << TWEA);
	}
	
	// включением приема очисткой TWINT
	TWCR |= (1 << TWINT);

	// ожидание завершения приема
	while (!(TWCR & (1<<TWINT)));

	if (STATUS_IS(expected_status)) {
		// полученные данные верны
		*data = TWDR;
		return I2C_STATUS_OK;
	}
	else {
		return I2C_STATUS_ERROR;
	}
}

/* Функция чтения нескольких байт подряд.*/
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