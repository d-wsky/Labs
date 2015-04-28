/*
 * SPI_DAC_Example.c
 *
 * Created: 23.10.2011 18:28:15
 *  Author: presentation
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

// Установка лог. "0" на выход SS микроконтроллера,
// включение режима передачи в ЦАПе
#define SS_transfer_data                   PORTB&=~(1<<1);        
// Установка лог. "1" на выход SS микроконтроллера,
// включение режима ожидании в ЦАПе
#define SS_waiting_transfer_data           PORTB|= (1<<1);
#define DAC_CS_ENABLE                      PORTB&=~(1<<4);//разрещение 
#define DAC_CS_DISABLE                     PORTB|= (1<<4);
#define DAC_LOAD_ON                        PORTB&=~(1<<5);// load always active
#define DAC_LOAD_OFF                       PORTB|= (1<<5);// загрузка запрещена 


void spi_init (void)
{
//  SPI initialization
//  SPI Type: Master
//  SPI Clock Rate: 2500,000 kHz
//  SPI Clock Phase: Cycle Start
//  SPI Clock Polarity: High
//  SPI Data Order: MSB First
  SPCR=0x50;
  SPSR=0x01;
  SS_waiting_transfer_data;
}    

//иницилизация ЦАПа
void dac_init(void)
{
    DAC_LOAD_OFF; 
    DAC_CS_DISABLE;
    DAC_LOAD_ON;
}   
 
//функция запуска передачи SPI с записью передающего бита в регистр передачи
void transfer_data_SPI(unsigned char data)
 {
     SS_transfer_data; 
     SPDR=data;
     while (!(SPSR & (1<<7)));
     SS_waiting_transfer_data;
 }   

//функция передачи данных в ЦАП
void DAC_set_level(unsigned char high,unsigned char low)
 {
    DAC_CS_ENABLE;
    transfer_data_SPI(high|0x30);
    _delay_us(50);
    transfer_data_SPI(low);
    _delay_us(50);
    DAC_CS_DISABLE ; 
 }


int main(void)
{
	int t=0;
    DDRB=0xFF;
	spi_init();
	dac_init();
	
	DDRC = 0xFF;
    SS_transfer_data;

	while (1) {
		t++;
		if (t>4095) t=0;
		DAC_CS_ENABLE;
		SPDR=(t>>8)|0x30;
		while(!(SPSR & (1<<SPIF))) ;
		SPDR=t;
		while(!(SPSR & (1<<SPIF))) ;
		DAC_CS_DISABLE;
	}
}