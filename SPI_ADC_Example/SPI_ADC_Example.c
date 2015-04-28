/*
 * SPI_ADC_Example.c
 *
 * Created: 02.09.2011 17:40:39
 *  Author: денис васильковский
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

// Вывод ножки MOSI
#define DD_MOSI      PB2
// Вывод ножки MISO
#define DD_SCK       PB1

/* Макрос команды перевода ножки CS АЦП в
состояние лог. "1" */
#define ADC_CS_HIGH  DDRB |= (1<<PB0); PORTB |= (1<<PB0);
/* Макрос команды перевода ножки CS АЦП в
состояние лог. "0" */
#define ADC_CS_LOW   DDRB |= (1<<PB0); PORTB &=~(1<<PB0);

/* Объединение, предназначеное для передачи
первого байта настройки АЦП */
union{
    struct {
	    char d2:1;
	    char single_channel:1;
	    char start_bit:1;
        char unused:5;
    } str;
	char chr;
} adc_ctrl0;

/* Объединение, предназначеное для передачи
второго байта настройки АЦП */
union {
    struct {
	    char unused:6;
	    char d0:1;
	    char d1:1;
    } str;
	char chr;
} adc_ctrl1;

/* Объединение, предназначеное для обработки
результатов преобразования */
union {
	struct {
		// Старшие биты преобразования
		char data:4;
		// Нулевой бит перед преобразованием
		char nul:1;
		// Пустые биты
		char unused:3;
	} str;
	// Результат в виде переменной
	char chr;
} adc_data_high;

// Младший байт преобразования
char adc_data_low;

/* Функция инициализации мастера шины SPI */
void SPI_MasterInit(void) {
    /* Установка выводов MOSI и SCK на вывод */
    DDRB = (1<<DD_MOSI)|(1<<DD_SCK);
    /* Включение SPI, режима ведущего, и установка
	частоты тактирования fclk/128 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
};

/* Функция передачи байта данных outData. Ожидает окончания
передачи и возвращает принятый по ножке MOSI байт */
unsigned char SPI_MasterTransmit(char outData) {
    /* Начало передачи */
    SPDR = outData;
    /* Ожидание окончания передачи */
    while(!(SPSR & (1<<SPIF))) ;
	return SPDR;
};

int main(void) {
	SPI_MasterInit();
	
	DDRC = 0xFF;
	DDRD = 0xFF;
	
    /* Если АЦП был запущен с ножкой CS в
	состоянии лог. "0", то нужно перещелкнуть
	ее и оставить в рабочем положении лог. "1" */
	ADC_CS_HIGH;
	ADC_CS_LOW;
	ADC_CS_HIGH;
	
	/* Заполнение контрольных байт */
	adc_ctrl0.str.unused=0;
	adc_ctrl0.str.start_bit=1;
	adc_ctrl0.str.single_channel=0;
	adc_ctrl0.str.d2=0;
	adc_ctrl1.str.d0=0;
	adc_ctrl1.str.d1=1;
	adc_ctrl1.str.unused=0;
	
    while(1) {
        /* Передача данных */ 
		ADC_CS_LOW;
	    SPI_MasterTransmit(adc_ctrl0.chr);
	    adc_data_high.chr=SPI_MasterTransmit(adc_ctrl1.chr);
	    adc_data_low=SPI_MasterTransmit(0);
		ADC_CS_HIGH;
		
		/* Отображение данных */
		PORTC = adc_data_high.str.data;
		PORTD = adc_data_low;
		
		_delay_ms(100);
    }
}