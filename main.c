/*
 * main.c
 *
 *  Created on: 29 gru 2022
 *      Author: mherc
 *
 *  Library demo
 */
#include "hd44780/hd44780.h"
#include <util/delay.h>
#include <stdbool.h>

typedef struct{
	char text[15];
	bool is_choosen;
}menu_line;


void print_line(menu_line line, int row){
	hd44780_locate(1, row);
	hd44780_print(line.text);
	if( line.is_choosen ){
		hd44780_locate(0, row);
		hd44780_print("\x7e");
	}
}


int main( void ){
	hd44780_init();
	menu_line menu[4] = {
			{"Sushi", true},
			{"Ramen", false},
	};
	print_line(menu[0], 0);
	print_line(menu[1], 1);
	while( 1 );
}
