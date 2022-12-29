/*
 * hd44780.c
 *
 *  Created on: 29 gru 2022
 *      Author: mherc
 */

#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>

#include "hd44780.h"

static void data_dir_out( void ){
	DDR(D4_PORT) |= (1<<D4_PIN);
	DDR(D5_PORT) |= (1<<D5_PIN);
	DDR(D6_PORT) |= (1<<D6_PIN);
	DDR(D7_PORT) |= (1<<D7_PIN);
}

static void data_dir_in( void ){
	DDR(D4_PORT) &= ~(1<<D4_PIN);
	DDR(D5_PORT) &= ~(1<<D5_PIN);
	DDR(D6_PORT) &= ~(1<<D6_PIN);
	DDR(D7_PORT) &= ~(1<<D7_PIN);
}

static void send_half( uint8_t _data ){
	if( _data & (1<<0) ) PORT(D4_PORT) |= (1<<D4_PIN);
	else PORT(D4_PORT) &= ~(1<<D4_PIN);

	if( _data & (1<<1) ) PORT(D5_PORT) |= (1<<D5_PIN);
	else PORT(D5_PORT) &= ~(1<<D5_PIN);

	if( _data & (1<<2) ) PORT(D6_PORT) |= (1<<D6_PIN);
	else PORT(D6_PORT) &= ~(1<<D6_PIN);

	if( _data & (1<<3) ) PORT(D7_PORT) |= (1<<D7_PIN);
	else PORT(D7_PORT) &= ~(1<<D7_PIN);
}

static uint8_t read_half( void ){
	uint8_t result = 0;
	if ( PIN(D4_PORT)&(1<<D4_PIN) ) result |= (1<<0);
	if ( PIN(D5_PORT)&(1<<D5_PIN) ) result |= (1<<1);
	if ( PIN(D6_PORT)&(1<<D6_PIN) ) result |= (1<<2);
	if ( PIN(D7_PORT)&(1<<D7_PIN) ) result |= (1<<3);
	return result;
}

static uint8_t read_byte( void ){
	uint8_t result = 0;
	data_dir_in();

	RW_SET;
	E_SET;
	result |= (read_half() << 4);
	E_RESET;

	E_SET;
	result |= read_half();
	E_RESET;

	RW_RESET;
	return result;
}

static bool check_bf(){
	RS_RESET;
	return (read_byte()&(1<<7));
}

static void send_byte( uint8_t data ){
	data_dir_out();
	RW_RESET;

	E_SET;
	send_half(data>>4);
	E_RESET;

	E_SET;
	send_half(data);
	E_RESET;

	while ( check_bf() ) {}
}

static void send_instruction( uint8_t cmd ){
	RS_RESET;
	send_byte(cmd);
}

static void send_data( uint8_t data ){
	RS_SET;
	send_byte(data);
}

void hd44780_locate( int x, int y ){
	uint8_t row;
	switch( y ){
	case 0: row=LINE1; break;
	case 1: row=LINE2; break;
	default: row = LINE1;
	}
	send_instruction( (0x80 + row + (uint8_t)x) );
}

void hd44780_print(char* str){
	while( *str ) send_data(*str++);
}

void hd44780_clear( void ){
	send_instruction( LCDC_CLS );
}

void hd44780_init( void ){
	data_dir_out();
	DDR(RS_PORT) |= (1<<RS_PIN);
	DDR(RW_PORT) |= (1<<RW_PIN);
	DDR(E_PORT) |= (1<<E_PIN);

	RS_SET;
	RW_SET;
	E_SET;

	_delay_ms(15);
	RS_RESET;
	RW_RESET;
	E_RESET;

	E_SET;
	send_half(0x03);
	E_RESET;
	_delay_ms(4.1);

	E_SET;
	send_half(0x03);
	E_RESET;
	_delay_us(100);

	E_SET;
	send_half(0x03);
	E_RESET;
	_delay_us(100);

	E_SET;
	send_half(0x02);
	E_RESET;
	_delay_us(100);

	send_instruction( LCDC_FUNC|LCDC_FUNC4B|LCDC_FUNC2L|LCDC_FUNC5x7 );
	send_instruction( LCDC_ONOFF|LCDC_CURSOROFF );
	send_instruction( LCDC_ONOFF|LCDC_DISPLAYON );
	send_instruction( LCDC_ENTRY|LCDC_ENTRYR );
	hd44780_clear();
}
