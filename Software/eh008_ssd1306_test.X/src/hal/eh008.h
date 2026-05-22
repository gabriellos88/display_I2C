/* 
 * File:	eh008.h
 * 
 * Author: 
 * Created on 21 May 2026
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

///////////////////////////////////////////////////////////////////////////////
// application  includes
#include "eh008_common.h"

///////////////////////////////////////////////////////////////////////////////
// system includes
#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// constants

//select display size 
#define SSD1306_128_64
//#define SSD1306_128_32

#if defined(SSD1306_128_64)
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#elif defined(SSD1306_128_32)
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 32
#endif

#define SSD1306_PAGE_HEIGHT 8
#define SSD1306_PAGE_COUNT (SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT)
#define SSD1306_BUFFER_SIZE (SSD1306_WIDTH * SSD1306_PAGE_COUNT)

#define SSD1306_I2C_ADDR_7BIT 0x3C
#define SSD1306_I2C_ADDR_WRITE (SSD1306_I2C_ADDR_7BIT << 1)

////////////////////////////////////////////////////////////////////////////////
// typedefs

typedef enum {
	SSD1306_COLOR_OFF = 0,
	SSD1306_COLOR_ON,
	SSD1306_COLOR_INVERT
} ssd1306_color_t;

////////////////////////////////////////////////////////////////////////////////
// public functions
void ssd1306_init(void);
void ssd1306_clear(void);
void ssd1306_fill(ssd1306_color_t color);
void ssd1306_update(void);

void ssd1306_draw_pixel(int16_t x, int16_t y, ssd1306_color_t color);
bool ssd1306_get_pixel(int16_t x, int16_t y);

void ssd1306_draw_hline(int16_t x, int16_t y, int16_t width, ssd1306_color_t color);
void ssd1306_draw_vline(int16_t x, int16_t y, int16_t height, ssd1306_color_t color);
void ssd1306_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, ssd1306_color_t color);
void ssd1306_draw_rect(int16_t x, int16_t y, int16_t width, int16_t height, ssd1306_color_t color);

#endif // __SSD1306_H__
