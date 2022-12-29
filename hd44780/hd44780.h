/*
 * hd44780.h
 *
 *  Created on: 29 gru 2022
 *      Author: mherc
 *
 *  ONLY FOR 2x16 with 4bit interface
 */

#ifndef HD44780_HD44780_H_
#define HD44780_HD44780_H_


/*----------------------------------------
 * CONSTANTS DEFINITIONS
 *
 *
 * Dimensions */
#define HEIGHT 2
#define WIDTH 16

/* Addresses */
#define LINE1 0x00
#define LINE2 0x40

/* Pinout and ports definitions */
#define RS_PORT A
#define RS_PIN 0
#define RW_PORT A
#define RW_PIN 1
#define E_PORT A
#define E_PIN 2

#define D4_PORT C
#define D4_PIN 2
#define D5_PORT C
#define D5_PIN 3
#define D6_PORT C
#define D6_PIN 4
#define D7_PORT C
#define D7_PIN 5
/*----------------------------------------*/

// Registers Macros
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)

#define PIN(x) SPIN(x)
#define SPIN(x) (PIN##x)

// Functional macros
#define RS_SET PORT(RS_PORT) |= (1<<RS_PIN)
#define RS_RESET PORT(RS_PORT) &= ~(1<<RS_PIN)

#define RW_SET PORT(RW_PORT) |= (1<<RW_PIN)
#define RW_RESET PORT(RW_PORT) &= ~(1<<RW_PIN)

#define E_SET PORT(E_PORT) |= (1<<E_PIN)
#define E_RESET PORT(E_PORT) &= ~(1<<E_PIN)

// HD44780 instructions
#define LCDC_CLS					0x01
#define LCDC_HOME					0x02
#define LCDC_ENTRY					0x04
	#define LCDC_ENTRYR					0x02
	#define LCDC_ENTRYL					0
	#define LCDC_MOVE					0x01
#define LCDC_ONOFF					0x08
	#define LCDC_DISPLAYON				0x04
	#define LCDC_CURSORON				0x02
	#define LCDC_CURSOROFF				0
	#define LCDC_BLINKON				0x01
#define LCDC_SHIFT					0x10
	#define LCDC_SHIFTDISP				0x08
	#define LCDC_SHIFTR					0x04
	#define LCDC_SHIFTL					0
#define LCDC_FUNC					0x20
	#define LCDC_FUNC8B					0x10
	#define LCDC_FUNC4B					0
	#define LCDC_FUNC2L					0x08
	#define LCDC_FUNC1L					0
	#define LCDC_FUNC5x10				0x04
	#define LCDC_FUNC5x7				0
#define LCDC_SET_CGRAM				0x40
#define LCDC_SET_DDRAM				0x80

// Public functions declaration
void hd44780_init( void );
void hd44780_clear( void );

void hd44780_locate( int x, int y );
void hd44780_print( char* str );


#endif /* HD44780_HD44780_H_ */
