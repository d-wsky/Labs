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
 * lcd.c
 *
 * Created: 23-Aug-16 00:28:08
 *  Author: Denis Vasilkovskii
 */ 

#include "lcd.h"
#include "gpio.h"
#include "delay.h"
#include "string.h"
#include <avr/pgmspace.h>


#define read_pin(pin_name)     ({ uint16_t word_val = pgm_read_word(&pinout_->pin_name); \
                                  *(Gpio_t *)&word_val;}) 

#define begin_pin(pin_name)    \
do \
{\
    uint16_t pin_ = pgm_read_word(&pinout_->pin_name); \
    uint16_t unused_pin = *(uint16_t *)&((Gpio_t)LCD_UNUSED_PIN);\
    if (pin_ != unused_pin) {\
        gpioPinModeSet(read_pin(pin_name), GPIO_MODE_OUT);\
    }\
} while (0);

#define clear_pin(pin_name)    \
do \
{ \
    gpioPinClear(read_pin(pin_name));\
} while (0);

#define set_pin(pin_name)    \
do \
{ \
    gpioPinSet(read_pin(pin_name));\
} while (0);

#define assign_pin(pin_name, value)   \
do \
{ \
    if (value) { \
        set_pin(pin_name);\
    }\
    else {\
        clear_pin(pin_name);\
    }\
} while (0);

#define PUTNIBBLE_DELAY          delayUs(500)

typedef enum {
    WIDTH_4BITS,
    WIDTH_8BITS,
} LcdDataWidth_t;

/* физические параметры дисплея */
#define HD44780_PHYSICAL_CMD        (1 << 5)
#define HD44780_5x8MATRIX           (0 << 2)
#define HD44780_5x10MATRIX          (1 << 2)
#define HD44780_1ROW                (0 << 3)
#define HD44780_2ROW                (1 << 3)
#define HD44780_4BIT_BUS            (0 << 4)
#define HD44780_8BIT_BUS            (1 << 4)
/* поддержка русской кодовой страницы WS0001 */
#define HD44780_RUS_FONT_TABLE      (2 << 0)

typedef enum {
    Phys1Row8BitsBus = HD44780_PHYSICAL_CMD | HD44780_8BIT_BUS | HD44780_1ROW | HD44780_5x8MATRIX | HD44780_RUS_FONT_TABLE,
    Phys1Row4BitsBus = HD44780_PHYSICAL_CMD | HD44780_4BIT_BUS | HD44780_1ROW | HD44780_5x8MATRIX | HD44780_RUS_FONT_TABLE,
    Phys2Row8BitsBus = HD44780_PHYSICAL_CMD | HD44780_8BIT_BUS | HD44780_2ROW | HD44780_5x8MATRIX | HD44780_RUS_FONT_TABLE,
    Phys2Row4BitsBus = HD44780_PHYSICAL_CMD | HD44780_4BIT_BUS | HD44780_2ROW | HD44780_5x8MATRIX | HD44780_RUS_FONT_TABLE,
} PhysicalParams_t;

/* направление вывода */
#define HD44780_OUT_CMD             (1 << 2)
#define HD44780_OUT_LEFT_TO_RIGHT   (1 << 1)
#define HD44780_OUT_RIGHT_TO_LEFT   (0 << 1)
#define HD44780_OUT_SHIFT_CURSOR    (0 << 0)
#define HD44780_OUT_SHIFT_DISPLAY   (1 << 0)

typedef enum {
    OutNorm   = HD44780_OUT_CMD | HD44780_OUT_LEFT_TO_RIGHT | HD44780_OUT_SHIFT_CURSOR,
    OutArabic = HD44780_OUT_CMD | HD44780_OUT_RIGHT_TO_LEFT | HD44780_OUT_SHIFT_CURSOR
} OutMode_t;

/* варианты отображения курсора */
#define HD44780_CURSOR_MODE_CMD     (1 << 3)
#define HD44780_DISPLAY_ON          (1 << 2)
#define HD44780_DISPLAY_OFF         (0 << 2)
#define HD44780_CURSOR_ON           (1 << 1)
#define HD44780_CURSOR_OFF          (0 << 1)
#define HD44780_BLINK_ON            (1 << 0)
#define HD44780_BLINK_OFF           (0 << 0)

typedef enum {
    DispOff          = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_OFF,
    CursorOff        = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF,
    CursorMode1      = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_ON  | HD44780_BLINK_OFF,
    CursorMode1Blink = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_ON  | HD44780_BLINK_ON,
    CursorMode2      = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF,
    CursorMode2Blink = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_ON
} CursorMode_t;

/* варианты перемещения курсора и дисплея */
#define HD44780_MOVE_CMD            (1 << 4)
#define HD44780_MOVE_DISPLAY        (1 << 3)
#define HD44780_MOVE_CURSOR         (0 << 3)
#define HD44780_MOVE_RIGHT          (1 << 2)
#define HD44780_MOVE_LEFT           (0 << 2)

typedef enum {
    MoveDisplayLeft  = HD44780_MOVE_CMD | HD44780_MOVE_DISPLAY | HD44780_MOVE_LEFT,
    MoveDisplayRight = HD44780_MOVE_CMD | HD44780_MOVE_DISPLAY | HD44780_MOVE_RIGHT,
    MoveCursorLeft   = HD44780_MOVE_CMD | HD44780_MOVE_CURSOR  | HD44780_MOVE_LEFT,
    MoveCursorRight  = HD44780_MOVE_CMD | HD44780_MOVE_CURSOR  | HD44780_MOVE_RIGHT,
} MoveAction_t;

#define HD44780_CLEAR_CMD           (1 << 0)
#define HD44780_HOME_CMD            (1 << 1)
#define HD44780_GOTO_CMD            (1 << 7)
#define HD44780_GOTO_CHAR_SPACE_CMD (1 << 6)
#define HD44780_GOTO_CHAR(n)        (HD44780_GOTO_CHAR_SPACE_CMD | (n << 3))

typedef enum {
    COMMAND_REGISTER = 0,
    DATA_REGISTER    = 1,
} LcdRegister_t;

static int lcdWriteToStream(char character, FILE *stream);

FILE lcd_iostream_ = FDEV_SETUP_STREAM(lcdWriteToStream, NULL, _FDEV_SETUP_WRITE);
static lcdPinout_t  const * pinout_ = NULL;
static CursorMode_t cursor_mode_ = CursorOff;
static OutMode_t    out_mode_ = OutNorm;

static void portInit() {
    begin_pin(rs);
    begin_pin(rw);
    begin_pin(e);
    begin_pin(d0);
    begin_pin(d1);
    begin_pin(d2);
    begin_pin(d3);
    begin_pin(d4);
    begin_pin(d5);
    begin_pin(d6);
    begin_pin(d7);
}

static void putNibble(uint8_t nibble) {
    set_pin(e);
    assign_pin(d4, nibble & _BV(0));
    assign_pin(d5, nibble & _BV(1));
    assign_pin(d6, nibble & _BV(2));
    assign_pin(d7, nibble & _BV(3));
    clear_pin(e);
    PUTNIBBLE_DELAY;
}

static LcdDataWidth_t getDataWidth() {
    Gpio_t d0 = read_pin(d0);
    return memcmp(&d0, &((Gpio_t)LCD_UNUSED_PIN), sizeof(Gpio_t)) == 0 ? WIDTH_4BITS : WIDTH_8BITS;
}

static void putbyte(char c, LcdRegister_t reg) {
    if (reg == COMMAND_REGISTER) {
        clear_pin(rs);
    }
    else {
        set_pin(rs);
    }
    
    if (getDataWidth() == WIDTH_4BITS) {
        putNibble(c >> 4);
        putNibble(c);
    }
    else {    
        set_pin(e);
        assign_pin(d0, c & _BV(0));
        assign_pin(d1, c & _BV(1));
        assign_pin(d2, c & _BV(2));
        assign_pin(d3, c & _BV(3));
        assign_pin(d4, c & _BV(4));
        assign_pin(d5, c & _BV(5));
        assign_pin(d6, c & _BV(6));
        assign_pin(d7, c & _BV(7));
        clear_pin(e);
        PUTNIBBLE_DELAY;
    }    
}

void lcdInit(const lcdPinout_t * pinout) {
    // todo assert(pinout != NULL);
    pinout_ = pinout;
    cursor_mode_ = HD44780_CURSOR_MODE_CMD | HD44780_DISPLAY_ON;
    // todo assert_static(sizeof(Gpio_t) == 2);
}

void lcdBegin(uint8_t cols, uint8_t rows) {
    // ширина дисплея никак не влияет на процедуру инициализации
    (void)cols;
    portInit();
    
    delayMs(100);
    // Передача команды сброса дисплея WS0001
    clear_pin(rs);
    putNibble(0x00);
    delayMs(5);
    putNibble(0x00);
    delayMs(5);
    putNibble(0x00);
    delayMs(5);
    putNibble(0x00);
    delayMs(5);
    putNibble(0x00);
    delayMs(5);
        
    // Выбор шины, дисплея
    if (getDataWidth() == WIDTH_4BITS) {
        PhysicalParams_t ph = rows == 1 ? Phys1Row4BitsBus : Phys2Row4BitsBus;
        putNibble(ph >> 4);
        delayUs(4100);
        putNibble(ph >> 4);
        delayUs(100);
        putNibble(ph & 0x0f);
        // \todo Здесь должна быть проверка флага занятости
        delayMs(10);
    }        
    else {
        PhysicalParams_t ph = rows == 1 ? Phys1Row8BitsBus : Phys2Row8BitsBus;
        putbyte(ph, COMMAND_REGISTER);
        delayUs(4100);
        putbyte(ph, COMMAND_REGISTER);
        delayUs(100);
        // \todo Здесь должна быть проверка флага занятости
        delayMs(10);
    }    
    
    // Запись режима курсора по-умолчанию
    putbyte(cursor_mode_, COMMAND_REGISTER);
    // Здесь должна быть проверка флага занятости
    delayMs(10);
        
    // Очистка дисплея
    lcdClear();
    // Здесь должна быть проверка флага занятости
    delayMs(10);

    // Режим вывода
    lcdLeftToRight();
        
    // Дополнительно: возврат в координату (0, 0)
    lcdHome();
}

void lcdHome() {
    putbyte(HD44780_HOME_CMD, COMMAND_REGISTER);
}

void lcdClear() {
    putbyte(HD44780_CLEAR_CMD, COMMAND_REGISTER);
    delayUs(1500);
}

void lcdCursor() {
    cursor_mode_ |= HD44780_CURSOR_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdNoCursor() {
    cursor_mode_ &= ~HD44780_CURSOR_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdBlink() {
    cursor_mode_ |= HD44780_BLINK_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdNoBlink() {
    cursor_mode_ &= ~HD44780_BLINK_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdDisplay() {
    cursor_mode_ |= HD44780_DISPLAY_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdNoDisplay() {
    cursor_mode_ &= ~HD44780_DISPLAY_ON;
    putbyte(cursor_mode_, COMMAND_REGISTER);
}

void lcdScrollDisplayLeft() {
    putbyte(MoveDisplayLeft, COMMAND_REGISTER);
}

void lcdScrollDisplayRight() {
    putbyte(MoveDisplayRight, COMMAND_REGISTER);
}

void lcdAutoscroll() {
    out_mode_ |= HD44780_OUT_SHIFT_DISPLAY;
    putbyte(out_mode_, COMMAND_REGISTER);
}

void lcdNoAutoscroll() {
    out_mode_ &= ~HD44780_OUT_SHIFT_DISPLAY;
    putbyte(out_mode_, COMMAND_REGISTER);
}

void lcdLeftToRight() {
    const OutMode_t out = OutNorm;
    putbyte(out, COMMAND_REGISTER);
}

void lcdRightToLeft() {
    const OutMode_t out = OutArabic;
    putbyte(out, COMMAND_REGISTER);
}

void lcdSetCursor(uint8_t col, uint8_t row) {
    uint8_t pos = (row << 6) + col;
    putbyte(HD44780_GOTO_CMD | pos, COMMAND_REGISTER);
}

int lcdWrite(uint8_t character) {
    putbyte(character, DATA_REGISTER);
    return 1;
}

static int lcdWriteToStream(char character, FILE *stream) {
    return lcdWrite(character);
}

uint8_t lcdPrintString(const char * str) {
    uint8_t written_counter = 0;
    while (*str) {
        written_counter += lcdWrite(*str++);
    }
    return written_counter;
}

void lcdCreateChar(uint8_t num, const uint8_t * data) {
    putbyte(HD44780_GOTO_CHAR(num), COMMAND_REGISTER);
    for (uint8_t i = 0; i < 8; i++) {
        putbyte(data[i], DATA_REGISTER);
    }
}

