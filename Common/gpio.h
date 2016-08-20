/*
 * gpio.h
 *
 * Created: 06-Mar-16 18:22:43
 *  Author: Denis Vasilkovsky
 *
 *   About: Базовый драйвер управления ножками ввода вывода.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
#include <assert.h>

/*! \brief Тип данных, используемый для определения режима ввода-вывода.*/
typedef enum {
    GPIO_MODE_IN = 0,   /*!< Режим ввода данных в микроконтроллер.*/
    GPIO_MODE_OUT = 1,  /*!< Режим вывода данных из микроконтроллера.*/
} GpioMode_t;

/*! \brief Тип данных, используемый для выбора тактовой скорости соединения.*/
typedef enum {
    GPIO_LEVEL_LOW = 0,  /*!< Низкий уровень напряжения на ножке.*/
    GPIO_LEVEL_HIGH = 1, /*!< Высокий уровень напряжения на ножке.*/
} GpioLevel_t;

/*! \brief Тип данных, используемый для выбора режима тактирования данных.*/
typedef enum {
    GPIO_PORTA = 0, /*!< Порт A.*/
    GPIO_PORTB,     /*!< Порт B.*/
    GPIO_PORTC,     /*!< Порт C.*/
    GPIO_PORTD,     /*!< Порт D.*/
    GPIO_PORTE,     /*!< Порт E.*/
    GPIO_PORTF,     /*!< Порт F.*/
    GPIO_PORTG,     /*!< Порт G.*/
} GpioPort_t;

typedef enum {
    GPIO_PIN0 = _BV(0),       /*!< Ножка 0 (младший бит порта).*/
    GPIO_PIN1 = _BV(1),       /*!< Ножка 1.*/
    GPIO_PIN2 = _BV(2),       /*!< Ножка 2.*/
    GPIO_PIN3 = _BV(3),       /*!< Ножка 3.*/
    GPIO_PIN4 = _BV(4),       /*!< Ножка 4.*/
    GPIO_PIN5 = _BV(5),       /*!< Ножка 5.*/
    GPIO_PIN6 = _BV(6),       /*!< Ножка 6.*/
    GPIO_PIN7 = _BV(7),       /*!< Ножка 7 (старший бит порта).*/
    GPIO_PIN_ALL = UINT8_MAX, /*!< Все ножки порта разом.*/
} GpioPin_t;

/*! \brief Функция установки высокого уровня напряжения на одной
или нескольких ножках одного порта.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_set(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция установки низкого уровня напряжения на одной
или нескольких ножках одного порта.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_clear(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция переключения текущего уровня напряжения на одной
или нескольких ножках одного порта на противоположный.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_toggle(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция установки новых значений напряжений на всех ножках
порта одновременно.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_value - это число будет выведено напрямую на весь порт.
*/
static inline void gpio_port_assign(GpioPort_t p, uint8_t pins_value);

/*! \brief Функция чтения текущего напряжения на одной из ножках порта.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pin_num - номер ножки, с которой будет произведено действие (0..7).

\return Переменную типа \ref GpioLevel_t, указывающую уровень на порте
в момент чтения.
*/
static inline GpioLevel_t gpio_pin_get(GpioPort_t p, GpioPin_t pin_num);

/*! \brief Функция настройки одной или нескольких ножек на ввод.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_mode_in(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция настройки одной или нескольких ножек на вывод.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_mode_out(GpioPort_t p, uint8_t pins_mask);


/*! \brief Функция, определяющая в каком режиме (ввод или вывод) находится
заданная ножка порта.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pin_num - номер ножки, с которой будет произведено действие (0..7).

\return Переменную типа \ref GpioMode_t, указывающую режим работы порта.
*/
static inline GpioMode_t gpio_pin_mode_get(GpioPort_t p, GpioPin_t pin_num);

/*! \brief Функция включения подтяжки к питанию на одной или нескольких
ножках порта.

\warning Ножки порта должны быть предварительно проинициализированы
на ввод с помощью функции \ref gpio_port_mode_in.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_pull_up_enable(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция отключения подтяжки к питанию на одной или нескольких
ножках порта.

\warning Ножки порта должны быть предварительно проинициализированы
на ввод с помощью функции \ref gpio_port_mode_in.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_pull_up_disable(GpioPort_t p, uint8_t pins_mask);

/* Реализация модуля находится в заголовочном файле,
потому что все функции объявлены с ключевым словом inline*/

/* Вспомогательные массивы, обеспечивающие выбор адреса
памяти, соответствующего порту*/
static volatile uint8_t * ports[] =
{
#ifdef PORTA
    &PORTA,
#else
    UINT16_MAX,
#endif

#ifdef PORTB
    &PORTB,
#else
    UINT16_MAX,
#endif

#ifdef PORTC
    &PORTC,
#else
    UINT16_MAX,
#endif

#ifdef PORTD
    &PORTD,
#else
    UINT16_MAX,
#endif

#ifdef PORTE
    &PORTE,
#else
    UINT16_MAX,
#endif

#ifdef PORTF
    &PORTF,
#else
    UINT16_MAX,
#endif

#ifdef PORTG
    &PORTG,
#else
    UINT16_MAX,
#endif
};

static const volatile uint8_t * pins[] =
{
#ifdef PINA
    &PINA,
#else
    UINT16_MAX,
#endif

#ifdef PINB
    &PINB,
#else
    UINT16_MAX,
#endif

#ifdef PINC
    &PINC,
#else
    UINT16_MAX,
#endif

#ifdef PIND
    &PIND,
#else
    UINT16_MAX,
#endif

#ifdef PINE
    &PINE,
#else
    UINT16_MAX,
#endif

#ifdef PINF
    &PINF,
#else
    UINT16_MAX,
#endif

#ifdef PING
    &PING,
#else
    UINT16_MAX,
#endif
};

static volatile uint8_t * dirs[8] =
{
#ifdef DDRA
    &DDRA,
#else
    UINT16_MAX,
#endif

#ifdef DDRB
    &DDRB,
#else
    UINT16_MAX,
#endif

#ifdef DDRC
    &DDRC,
#else
    UINT16_MAX,
#endif

#ifdef DDRD
    &DDRD,
#else
    UINT16_MAX,
#endif

#ifdef DDRE
    &DDRE,
#else
    UINT16_MAX,
#endif

#ifdef DDRF
    &DDRF,
#else
    UINT16_MAX,
#endif

#ifdef DDRG
    &DDRG,
#else
    UINT16_MAX,
#endif
};

static inline void gpio_pin_set(GpioPort_t p, uint8_t pins_mask)
{
    // todo вход в критическую секцию здесь и ниже
    *ports[p] |= pins_mask;
    // todo выход из критической секции здесь и ниже
}
static inline void gpio_pin_clear(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] &= ~pins_mask;
}

static inline void gpio_pin_toggle(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] ^= pins_mask;
}

static inline void gpio_port_assign(GpioPort_t p, uint8_t pins_value)
{
    *ports[p] = pins_value;
}

static inline GpioLevel_t gpio_pin_get(GpioPort_t p, GpioPin_t pin_num)
{
    return (GpioLevel_t)(!!(*pins[p] & _BV(pin_num)));
}

static inline void gpio_pin_mode_in(GpioPort_t p, uint8_t pins_mask)
{
    *dirs[p] &= ~pins_mask;
}

static inline void gpio_pin_mode_out(GpioPort_t p, uint8_t pins_mask)
{
    *dirs[p] |= pins_mask;
}

static inline GpioMode_t gpio_pin_mode_get(GpioPort_t p, GpioPin_t pin_num)
{
    return (GpioMode_t)(!!(*dirs[p] & pin_num));
}

static inline void gpio_pin_pull_up_enable(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] |= pins_mask;
}
static inline void gpio_pin_pull_up_disable(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] &= ~pins_mask;
}

#endif /* GPIO_H_ */
