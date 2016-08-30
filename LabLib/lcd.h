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
 * lcd.h
 *
 * Created: 23-Aug-16 00:04:40
 *  Author: Denis Vasilkovskii
 */ 


#ifndef LABLIB_LCD_H
#define LABLIB_LCD_H

#include "gpio.h"
#include <stdio.h>
#include <avr/pgmspace.h>

/*! \brief Экземпляр такой структуры определяет вариант подключения ЖКИ к
микроконтроллеру. ВАЖНО! Не используемые ножки контроллера ЖКИ должны быть
объявлены с макросом \ref LCD_UNUSED_PIN.

Количество ножек, используемых для подключения ЖКИ к микроконтроллеру может
быть разным. Во-первых, может различаться ширина шины данных:
  
  * 8 бит. В этом случае все ножки данных задействованы;
  * 4 бита. В этом случае ножки d0-d3 должны быть объявлены как LCD_UNUSED_PIN.

Во-вторых, подключение ножки rw не обязательно. Если она к микроконтроллеру
не подключена (при инициализации указана как LCD_UNUSED_PIN), то невозможно
осуществить чтение данных из контроллера ЖКИ.
*/
typedef struct {
    Gpio_t rs;  /*!< Ножка выбора регистра RS.*/
    Gpio_t e;   /*!< Ножка стробирования E.*/
    Gpio_t rw;  /*!< Ножка чтения-записи R/W.*/
    Gpio_t d0;  /*!< Ножка шины данных D0.*/
    Gpio_t d1;  /*!< Ножка шины данных D1.*/
    Gpio_t d2;  /*!< Ножка шины данных D2.*/
    Gpio_t d3;  /*!< Ножка шины данных D3.*/
    Gpio_t d4;  /*!< Ножка шины данных D4.*/
    Gpio_t d5;  /*!< Ножка шины данных D5.*/
    Gpio_t d6;  /*!< Ножка шины данных D6.*/
    Gpio_t d7;  /*!< Ножка шины данных D7.*/
} PROGMEM lcdPinout_t;

/*! \brief Внутренняя переменная, которая используется при перенаправлении
\a printf в ЖКИ индикатор.*/
extern FILE lcd_iostream_;

/*! \brief Макрос, используемый для того, чтобы показать, что ножка ЖКИ не
используется.*/
#define LCD_UNUSED_PIN       {.port = 0xFF, .pin = 0xFF}

/*! \brief Инициализация программного модуля LCD. Эту функцию следует вызвать
до функции \ref lcdBegin().

\param pinout - указатель на структуру, содержащую схему подключения ЖКИ к
микроконтроллеру.
*/
void lcdInit(const lcdPinout_t * pinout);

/*! \brief Инициализация интерфейса общения с контроллером ЖКИ, а также
установка физических параметров дисплея (ширина и высота). Эту функцию 
следует вызывать ДО начала передачи данных в ЖКИ.

\param cols - Количество символов в строке дисплея (чаще всего 16).
\param rows - Количество строк в дисплее (чаще всего две).
*/
void lcdBegin(uint8_t cols, uint8_t rows);
              
/*! \brief Очищает содержимое экрана и перемещает курсор в левую верхнуюю
позицию.
*/
void lcdClear();

/*! \brief Перемещает курсор в верхнюю левую позицию. Для того, чтобы очистить
содержимое экрана следует воспользоваться функцией \ref lcdClear().
*/
void lcdHome();

/*! \brief Устанавливает положение курсора, то есть указаывает знакоместо в
котором появится следующий символ, выведенный на экран.

\param col - Номер символа (колонки), начиная с нуля.
\param row - Номер строки, начиная с нуля.
*/
void lcdSetCursor(uint8_t col, uint8_t row);

/*! \brief Запись символа на экран ЖКИ.

\param character - код символа, который надо отобразить на экране ЖКИ.

Символ, посылаемый с помощью этой команды не преобразуется в строку, а
отображается как есть, в той кодовой странице, которая "зашита" в контроллер
ЖКИ. Например:

\code
lcdWrite('d'); // отображает 'd'
lcdWrite(100); // отображает 'd' (ascii код 100)
lcdWrite(3);   // отображает пользовательский символ с кодом '3'
\endcode

\return Всегда 1.
*/
int lcdWrite(uint8_t character);

/*! \brief Внутренний макрос, позволяющий сделать вывод информации на дисплей
независимым от типа данных.*/
#define printf_dec_format(x)                                 \
                    _Generic((x),                            \
                             char: "%c",                     \
                             const char *: "%s",             \
                             signed char: "%hhd",            \
                             unsigned char: "%hhu",          \
                             signed short: "%hd",            \
                             unsigned short: "%hu",          \
                             signed int: "%d",               \
                             unsigned int: "%u",             \
                             long int: "%ld",                \
                             unsigned long int: "%lu",       \
                             long long int: "%lld",          \
                             unsigned long long int: "%llu", \
                             float: "%f",                    \
                             char *: "%s",                   \
                             void *: "%p")

/*! \brief Функция вывода текста на дисплей. Данная функция поддерживает
любой встроенный тип данных и автоматически преобразует целочисленные типы
в строку, содержащую их двоичное представление. Например:

\code
lcdPrint("Hello world!"); // выводит "Hello world!"
lcdPrint(42);             // выводит "42"
\endcode
*/
#define lcdPrint(x)                                         \
                       do {                                 \
                           FILE * stdout_copy_ = stdout;    \
                           stdout = &lcd_iostream_;         \
                           printf(printf_dec_format(x), x); \
                           stdout = stdout_copy_;           \
                       } while (0);

/*! \brief Включает отображение курсора (знака подчеркивания) на экране.*/
void lcdCursor();

/*! \brief Выключает отображение курсора (знака подчеркивания) на экране.*/
void lcdNoCursor();

/*! \brief Включает режим моргания курсора. Результа использования в комбинации
с функцией \ref lcdCursor() зависит от конкретной модели дисплея.
*/
void lcdBlink();

/*! \brief Выключение режима моргания курсора.*/
void lcdNoBlink();

/*! \brief Включение режима отображения информации на дисплее.*/
void lcdDisplay();

/*! \brief Выключение отображения информации на дисплее. Данные в оперативной
памяти дисплея остаются, поэтому вызов функции \ref lcdDisplay() возвращает
дисплей к тому же состоянию, которое было до выключения вывода.
*/
void lcdNoDisplay();

/*! \brief Прокручивает видимую область дисплея вправо, создавая эффект
движения текста влево.
*/
void lcdScrollDisplayLeft();

/*! \brief Прокручивает видимую область дисплея влево, создавая эффект
движения текста вправо.
*/
void lcdScrollDisplayRight();

/*! \brief Включение режима автопрокручивания дисплея. В этом режиме при
выводе информации смещается также видимая область, что создает эффект
прокручивания.
*/
void lcdAutoscroll();

/*! \brief Выключения режима автопрокручивания дисплея.*/
void lcdNoAutoscroll();

/*! \brief Режим вывода текста слева направо.*/
void lcdLeftToRight();

/*! \brief Режим вывода текста справа налево.*/
void lcdRightToLeft();

/*! \brief Функция определения собственного символа внутри знакогенератора
модуля ЖКИ.

\param num - номер символа (от 0 до 7).
\param data - указатель на массив, содержащий графическое описание
              пользовательского символа. Символ кодируется построчно, из
              8 бит используется пять и логическая единица означает, что
              данный пиксель будет включен.

\details Если символ определяется в процессе работы с дисплеем, то перед 
выводом информации на дисплей необходимо вызвать любую из функций, задающих 
положение курсора дисплея: \ref lcdHome(), \ref lcdClear(), 
\ref lcdSetCursor(). В противном случае, запись будет произведена в CGRAM 
дисплея и на экране ничего не появится.
*/
void lcdCreateChar(uint8_t num, const uint8_t * data);

#endif /* LABLIB_LCD_H */