/* 
 * Copyright (c) 2016, Denis Vasilkovskii
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      - Neither the name of the OINPE NRNU MEPhI nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * digital.h
 *
 * Created: 08-Aug-16 23:35:46
 *  Author: Denis Vasilkovskii
 *
 *   About: Драйвер управления ножками ввода вывода.
 */

#ifndef LABLIB_GPIO_H
#define LABLIB_GPIO_H

#include <avr/io.h>

/*! \brief Тип данных, используемый для определения режима ввода-вывода.*/
typedef enum {
    GPIO_MODE_IN = 0,         /*!< Режим ввода данных в микроконтроллер.*/
    GPIO_MODE_IN_PULLUP = 1,  /*!< Режим ввода данных в микроконтроллер с 
                                   включенной встроенной подтяжкой ножки
                                   к питанию.*/
    GPIO_MODE_OUT = 2,        /*!< Режим вывода данных из микроконтроллера.*/
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
static inline void gpioPinSet(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция установки низкого уровня напряжения на одной
или нескольких ножках одного порта.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpioPinClear(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция переключения текущего уровня напряжения на одной
или нескольких ножках одного порта на противоположный.

\warning Ножки порта должны быть предварительно проинициализированы
на вывод с помощью функции \ref gpio_port_mode_out.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
*/
static inline void gpioPinToggle(GpioPort_t p, uint8_t pins_mask);

/*! \brief Функция настройки режима одной или нескольких ножек порта.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pins_mask - набор ножек, с которыми будет произведено действие.
Например: GPIO_PIN0, 1 << PB1, GPIO_PIN6 | GPIO_PIN7, GPIO_PIN_ALL.
\param mode - режим работы ножки, задаваемый переменной типа \ref GpioMode_t.
*/
static inline void gpioPinModeSet(GpioPort_t p, uint8_t pins_mask, GpioMode_t mode);

/*! \brief Функция считывания состояния ножки порта.

\param p - номер порта, задаваемый с помощью типа \ref GpioPort_t.
\param pin_num - Номер ножки для чтения. Например: GPIO_PIN0, 1 << PB1.
*/
static inline GpioLevel_t gpioPinGet(GpioPort_t p, GpioPin_t pin_num);


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

static volatile uint8_t * dirs[] =
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

static inline void gpioPinSet(GpioPort_t p, uint8_t pins_mask)
{
    // todo вход в критическую секцию здесь и ниже
    *ports[p] |= pins_mask;
    // todo выход из критической секции здесь и ниже
}
static inline void gpioPinClear(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] &= ~pins_mask;
}

static inline void gpioPinToggle(GpioPort_t p, uint8_t pins_mask)
{
    *ports[p] ^= pins_mask;
}

static inline GpioLevel_t gpioPinGet(GpioPort_t p, GpioPin_t pin_num)
{
    return (*pins[p]) & pin_num ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;
}

static inline void gpioPinModeSet(GpioPort_t p, uint8_t pins_mask, GpioMode_t mode)
{
    switch (mode) {
        case GPIO_MODE_IN:
            *dirs[p] &= ~pins_mask;
            *ports[p] &= ~pins_mask;
            break;
        case GPIO_MODE_IN_PULLUP:
            *dirs[p] &= ~pins_mask;
            *ports[p] |= pins_mask;
            break;
        case GPIO_MODE_OUT:
            *dirs[p] |= pins_mask;
            // со значением ports - будь, что будет
            break;
    }
}

#endif /* LABLIB_GPIO_H */