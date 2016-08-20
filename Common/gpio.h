/*
 * gpio.h
 *
 * Created: 06-Mar-16 18:22:43
 *  Author: Denis Vasilkovsky
 *
 *   About: ������� ������� ���������� ������� ����� ������.
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <avr/io.h>
#include <assert.h>

/*! \brief ��� ������, ������������ ��� ����������� ������ �����-������.*/
typedef enum {
    GPIO_MODE_IN = 0,   /*!< ����� ����� ������ � ���������������.*/
    GPIO_MODE_OUT = 1,  /*!< ����� ������ ������ �� ����������������.*/
} GpioMode_t;

/*! \brief ��� ������, ������������ ��� ������ �������� �������� ����������.*/
typedef enum {
    GPIO_LEVEL_LOW = 0,  /*!< ������ ������� ���������� �� �����.*/
    GPIO_LEVEL_HIGH = 1, /*!< ������� ������� ���������� �� �����.*/
} GpioLevel_t;

/*! \brief ��� ������, ������������ ��� ������ ������ ������������ ������.*/
typedef enum {
    GPIO_PORTA = 0, /*!< ���� A.*/
    GPIO_PORTB,     /*!< ���� B.*/
    GPIO_PORTC,     /*!< ���� C.*/
    GPIO_PORTD,     /*!< ���� D.*/
    GPIO_PORTE,     /*!< ���� E.*/
    GPIO_PORTF,     /*!< ���� F.*/
    GPIO_PORTG,     /*!< ���� G.*/
} GpioPort_t;

typedef enum {
    GPIO_PIN0 = _BV(0),       /*!< ����� 0 (������� ��� �����).*/
    GPIO_PIN1 = _BV(1),       /*!< ����� 1.*/
    GPIO_PIN2 = _BV(2),       /*!< ����� 2.*/
    GPIO_PIN3 = _BV(3),       /*!< ����� 3.*/
    GPIO_PIN4 = _BV(4),       /*!< ����� 4.*/
    GPIO_PIN5 = _BV(5),       /*!< ����� 5.*/
    GPIO_PIN6 = _BV(6),       /*!< ����� 6.*/
    GPIO_PIN7 = _BV(7),       /*!< ����� 7 (������� ��� �����).*/
    GPIO_PIN_ALL = UINT8_MAX, /*!< ��� ����� ����� �����.*/
} GpioPin_t;

/*! \brief ������� ��������� �������� ������ ���������� �� �����
��� ���������� ������ ������ �����.

\warning ����� ����� ������ ���� �������������� �������������������
�� ����� � ������� ������� \ref gpio_port_mode_out.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_set(GpioPort_t p, uint8_t pins_mask);

/*! \brief ������� ��������� ������� ������ ���������� �� �����
��� ���������� ������ ������ �����.

\warning ����� ����� ������ ���� �������������� �������������������
�� ����� � ������� ������� \ref gpio_port_mode_out.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_clear(GpioPort_t p, uint8_t pins_mask);

/*! \brief ������� ������������ �������� ������ ���������� �� �����
��� ���������� ������ ������ ����� �� ���������������.

\warning ����� ����� ������ ���� �������������� �������������������
�� ����� � ������� ������� \ref gpio_port_mode_out.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_toggle(GpioPort_t p, uint8_t pins_mask);

/*! \brief ������� ��������� ����� �������� ���������� �� ���� ������
����� ������������.

\warning ����� ����� ������ ���� �������������� �������������������
�� ����� � ������� ������� \ref gpio_port_mode_out.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_value - ��� ����� ����� �������� �������� �� ���� ����.
*/
static inline void gpio_port_assign(GpioPort_t p, uint8_t pins_value);

/*! \brief ������� ������ �������� ���������� �� ����� �� ������ �����.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pin_num - ����� �����, � ������� ����� ����������� �������� (0..7).

\return ���������� ���� \ref GpioLevel_t, ����������� ������� �� �����
� ������ ������.
*/
static inline GpioLevel_t gpio_pin_get(GpioPort_t p, GpioPin_t pin_num);

/*! \brief ������� ��������� ����� ��� ���������� ����� �� ����.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_mode_in(GpioPort_t p, uint8_t pins_mask);

/*! \brief ������� ��������� ����� ��� ���������� ����� �� �����.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_mode_out(GpioPort_t p, uint8_t pins_mask);


/*! \brief �������, ������������ � ����� ������ (���� ��� �����) ���������
�������� ����� �����.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pin_num - ����� �����, � ������� ����� ����������� �������� (0..7).

\return ���������� ���� \ref GpioMode_t, ����������� ����� ������ �����.
*/
static inline GpioMode_t gpio_pin_mode_get(GpioPort_t p, GpioPin_t pin_num);

/*! \brief ������� ��������� �������� � ������� �� ����� ��� ����������
������ �����.

\warning ����� ����� ������ ���� �������������� �������������������
�� ���� � ������� ������� \ref gpio_port_mode_in.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_pull_up_enable(GpioPort_t p, uint8_t pins_mask);

/*! \brief ������� ���������� �������� � ������� �� ����� ��� ����������
������ �����.

\warning ����� ����� ������ ���� �������������� �������������������
�� ���� � ������� ������� \ref gpio_port_mode_in.

\param p - ����� �����, ���������� � ������� ���� \ref GpioPort_t.
\param pins_mask - ����� �����, � �������� ����� ����������� ��������.
��������: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpio_pin_pull_up_disable(GpioPort_t p, uint8_t pins_mask);

/* ���������� ������ ��������� � ������������ �����,
������ ��� ��� ������� ��������� � �������� ������ inline*/

/* ��������������� �������, �������������� ����� ������
������, ���������������� �����*/
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
    // todo ���� � ����������� ������ ����� � ����
    *ports[p] |= pins_mask;
    // todo ����� �� ����������� ������ ����� � ����
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
