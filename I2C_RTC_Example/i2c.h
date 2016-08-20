/*
 * i2c.h
 *
 * Created: 12-Aug-15 22:52:04
 *  Author: Денис Васильковский
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <stdlib.h>

/*! \brief Удобный макрос для установки скорости соединения.*/
#define TWBR_FROM_HZ(x)  ( (F_CPU - 16UL * x) / (2UL * x * 1) + 1)

/*! \brief Этот тип данных используется для информирования
внешнего кода об успешности операции.*/
typedef enum {
	I2C_STATUS_OK,    /*!< Успешное исполнение функции.*/
	I2C_STATUS_ERROR  /*!< Исполнение функции было неуспешно.*/
} i2c_status_t;

/*! \brief С помощью этого кода можно выбрать какой уровень будет
у сигнала подтверждения.*/
typedef enum {
	I2C_ACK, /*!< Установка бита подтверждения (\a ACK)
	              после завершения приема.*/
	I2C_NACK /*!< Отсутствие бита подтверждение (\a NACK)
	              после завершения приема.*/
} i2c_ack_t;

/*! \brief Удобный макрос для выполнения одного действия внутри
функции, многократно обращающейся к шине I2C и желающей немедленного
выхода в случае возникновения проблем.

\param x - любая из операций данного драйвера, возвращающая статус.

Пример использования:

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

/*! \brief Инициализация внутреннего приемопередатчика I2C
микроконтроллера на работу в режиме 400 кГц и включение
его в работу.*/
void i2c_init();

/*! \brief Установка состояния \a START на шине I2C.

\return I2C_STATUS_OK - нет ошибки, I2C_STATUS_ERROR - установка состояния
потерпела неудачу.*/
i2c_status_t i2c_start();

/*! \brief Установка состояния \a REPEAT \a START на шине I2C.

\return I2C_STATUS_OK - нет ошибки, I2C_STATUS_ERROR - установка состояния
потерпела неудачу.*/
i2c_status_t i2c_repeat_start(void);

/*! \brief Установка состояния \a STOP на шине I2C.

\return Всегда I2C_STATUS_OK.*/
i2c_status_t i2c_stop();

/*! \brief Передача адреса и команды чтения/записи по шине I2C.

\param addr - передаваемый адрес.

\return I2C_STATUS_OK - нет ошибки, I2C_STATUS_ERROR - передача данных
потерпела неудачу.*/
i2c_status_t i2c_write_addr(unsigned char addr);

/*! \brief Передача байта данных по шине I2C в блокирующем режиме.

\param data - передаваемые данные.

\return I2C_STATUS_OK - нет ошибки, I2C_STATUS_ERROR - установка состояния
потерпела неудачу (проверка внутреннего флага #I2C_MAS_TX_DATA_ACK).*/
i2c_status_t i2c_write_byte(unsigned char data);

/*! \brief Чтение байта данных с шины I2C.

\param data - указатель на переменную, куда будут положены принятые данные.
\param ack - значение переменной указывает, устанавливать ли флаг
подтверждения (ACK) после прочтения. В случае, если параметр установлен
в #I2C_MAS_RX_DATA_ACK, означает, что сеанс связи продолжается и за этим
байтом последуют другие. В случае, если параметр установлен в
#I2C_MAS_RX_DATA_NACK дальнейшего обмена не будет и нужно будет выбирать
адрес slave-устройства заново.

\return I2C_STATUS_OK - нет ошибки, I2C_STATUS_ERROR - передача данных
потерпела неудачу.*/
i2c_status_t i2c_read_byte(unsigned char* data, i2c_ack_t ack);

/*! \brief Функция чтения нескольких байт подряд.

\param data - указатель на начало области памяти, куда будет положен результат
чтения.
\param len - количество байт вычитываемых из шины.

\return Количество верно полученных байт

Данная функция отправляет ACK на все принимаемые байты и отправляет NACK
на последний принятый байт, завершая сеанс связи.*/
size_t i2c_read_multiple(unsigned char* data, size_t len);

/*! \brief Функция записи нескольких байт подряд.

\param data - указатель на начало области памяти, откуда берется буфер на запись.
\param len - количество байт записываемых в шину.

\return Количество верно отправленных байт

В случае отсутствия бита подтверждения от ведомого устройства, данная
функция рвет передачу.*/
size_t i2c_write_multiple(unsigned char* data, const size_t len);

#endif /* I2C_H_ */
