/*
//     _____         __  __ ____       _ _ _                            
//    / ____|  /\   |  \/  |  _ \     | | (_)                           
//   | (___   /  \  | \  / | |_) | ___| | |_ _ __  _______  _ __   __ _ 
//    \___ \ / /\ \ | |\/| |  _ < / _ \ | | | '_ \|_  / _ \| '_ \ / _` |
//    ____) / ____ \| |  | | |_) |  __/ | | | | | |/ / (_) | | | | (_| |
//   |_____/_/    \_\_|  |_|____/ \___|_|_|_|_| |_/___\___/|_| |_|\__,_|
//
 * File: main.c
 * Authors: JZK and Filip (30 May 2021 on forum.mikroe.com)
	 restructured by Gabriele Losa on 21 May 2026
 * Description: use an OLED display (SSD1306 128x64) with I2C on PIC18F45K22
 */

////////////////////////////////////////////////////////////////////////////////
// application includes
#include "mapping.h"

////////////////////////////////////////////////////////////////////////////////
// system includes
#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// private functions

//------------------------------------------------------------------------------
static void draw_pixel_demo(void)
//------------------------------------------------------------------------------
{
	uint8_t x;
	uint8_t y;

	display_clear();
	
	// draw external perimeter
	display_rect(0, 0, SSD1306_WIDTH, SSD1306_HEIGHT, DISPLAY_COLOR_ON);
	
	// draw lines in the corners
	display_line(0, 0, SSD1306_WIDTH - 1, SSD1306_HEIGHT - 1, DISPLAY_COLOR_ON);
	display_line(0, SSD1306_HEIGHT - 1, SSD1306_WIDTH - 1, 0, DISPLAY_COLOR_ON);

	// draw a dot grid
	for (y = 4; y < SSD1306_HEIGHT; y += 8) {
		for (x = 4; x < SSD1306_WIDTH; x += 8) {
			display_pixel(x, y, DISPLAY_COLOR_ON);
		}
	}

	if (display_pixel_get(4, 4) == true) {
		display_pixel(4, 4, DISPLAY_COLOR_INVERT);
	}

	display_update();
}

//------------------------------------------------------------------------------
void main(void)
//------------------------------------------------------------------------------
{
	hw_init();
	
	draw_pixel_demo();

	while (true) {
		
	}
}
