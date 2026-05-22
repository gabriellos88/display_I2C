/* 
 * File:	eh008.c
 * 
 * Author: 
 * Created on 21 May 2026
 */

////////////////////////////////////////////////////////////////////////////////
// application includes
#include "eh008.h"
#include "eh008_i2c.h"
#include "eh008_common.h"


////////////////////////////////////////////////////////////////////////////////
// system includes
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// costants

#define SSD1306_CONTROL_COMMAND 0x00
#define SSD1306_CONTROL_DATA 0x40
#define SSD1306_DATA_CHUNK_SIZE 16

#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22

#define SSD1306_RST_LAT LATCbits.LATC0
#define SSD1306_RST_TRIS TRISCbits.TRISC0

////////////////////////////////////////////////////////////////////////////////
//static variables

static uint8_t framebuffer[SSD1306_BUFFER_SIZE];


////////////////////////////////////////////////////////////////////////////////
// private functions

//------------------------------------------------------------------------------
static void ssd1306_reset_init(void)
//------------------------------------------------------------------------------
{
	SSD1306_RST_LAT = 1;
	SSD1306_RST_TRIS = GPIO_OUTPUT;
}

//------------------------------------------------------------------------------
static void ssd1306_reset(void)
//------------------------------------------------------------------------------
{
	SSD1306_RST_LAT = 0;
	__delay_ms(10);
	SSD1306_RST_LAT = 1;
	__delay_ms(100);
}

//------------------------------------------------------------------------------
static void ssd1306_write_command(uint8_t command)
//------------------------------------------------------------------------------
{
	eh008_i2c_master_start();
	eh008_i2c_master_write(SSD1306_I2C_ADDR_WRITE);
	eh008_i2c_master_write(SSD1306_CONTROL_COMMAND);
	eh008_i2c_master_write(command);
	eh008_i2c_master_stop();
}

//------------------------------------------------------------------------------
static void ssd1306_write_data(const uint8_t *data, uint8_t length)
//------------------------------------------------------------------------------
{
	uint8_t index;

	if (data == NULL || length == 0) {
		return;
	}

	eh008_i2c_master_start();
	eh008_i2c_master_write(SSD1306_I2C_ADDR_WRITE);
	eh008_i2c_master_write(SSD1306_CONTROL_DATA);
	for (index = 0; index < length; index++) {
		eh008_i2c_master_write(data[index]);
	}
	eh008_i2c_master_stop();
}

//------------------------------------------------------------------------------
void ssd1306_init(void)
//------------------------------------------------------------------------------
{
#ifdef FEAT_I2C
	eh008_mssp1_init();
#endif
	ssd1306_reset_init();
	ssd1306_reset();

	ssd1306_write_command(SSD1306_DISPLAYOFF);
	ssd1306_write_command(SSD1306_SETDISPLAYCLOCKDIV);
	ssd1306_write_command(0x80);
	ssd1306_write_command(SSD1306_SETMULTIPLEX);
	ssd1306_write_command(SSD1306_HEIGHT - 1);
	ssd1306_write_command(SSD1306_SETDISPLAYOFFSET);
	ssd1306_write_command(0x00);
	ssd1306_write_command(SSD1306_SETSTARTLINE | 0x00);
	ssd1306_write_command(SSD1306_CHARGEPUMP);
	ssd1306_write_command(0x14);
	ssd1306_write_command(SSD1306_MEMORYMODE);
	ssd1306_write_command(0x00); // Horizontal addressing mode.
	ssd1306_write_command(SSD1306_SEGREMAP | 0x01);
	ssd1306_write_command(SSD1306_COMSCANDEC);

#if defined(SSD1306_128_32)
	ssd1306_write_command(SSD1306_SETCOMPINS);
	ssd1306_write_command(0x02);
	ssd1306_write_command(SSD1306_SETCONTRAST);
	ssd1306_write_command(0x8F);
#elif defined(SSD1306_128_64)
	ssd1306_write_command(SSD1306_SETCOMPINS);
	ssd1306_write_command(0x12);
	ssd1306_write_command(SSD1306_SETCONTRAST);
	ssd1306_write_command(0xCF);
#endif

	ssd1306_write_command(SSD1306_SETPRECHARGE);
	ssd1306_write_command(0xF1);
	ssd1306_write_command(SSD1306_SETVCOMDETECT);
	ssd1306_write_command(0x40);
	ssd1306_write_command(SSD1306_DISPLAYALLON_RESUME);
	ssd1306_write_command(SSD1306_NORMALDISPLAY);
	ssd1306_write_command(SSD1306_DEACTIVATE_SCROLL);
	ssd1306_write_command(SSD1306_DISPLAYON);

	ssd1306_clear();
	ssd1306_update();
}

//------------------------------------------------------------------------------
void ssd1306_clear(void)
//------------------------------------------------------------------------------
{
	memset(framebuffer, 0x00, sizeof(framebuffer));
}

//------------------------------------------------------------------------------
void ssd1306_fill(ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	memset(framebuffer, color == SSD1306_COLOR_OFF ? 0x00 : 0xFF, sizeof(framebuffer));
}

//------------------------------------------------------------------------------
void ssd1306_update(void)
//------------------------------------------------------------------------------
{
	uint16_t index;

	ssd1306_write_command(SSD1306_COLUMNADDR);
	ssd1306_write_command(0);
	ssd1306_write_command(SSD1306_WIDTH - 1);
	ssd1306_write_command(SSD1306_PAGEADDR);
	ssd1306_write_command(0);
	ssd1306_write_command(SSD1306_PAGE_COUNT - 1);

	for (index = 0; index < SSD1306_BUFFER_SIZE; index += SSD1306_DATA_CHUNK_SIZE) {
		ssd1306_write_data(&framebuffer[index], SSD1306_DATA_CHUNK_SIZE);
	}
}

//------------------------------------------------------------------------------
void ssd1306_draw_pixel(int16_t x, int16_t y, ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	uint16_t index;
	uint8_t mask;

	if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) {
		return;
	}

	index = (uint16_t)x + ((uint16_t)y / SSD1306_PAGE_HEIGHT) * SSD1306_WIDTH;
	mask = (uint8_t)(1U << (y % SSD1306_PAGE_HEIGHT));

	switch (color) {
		case SSD1306_COLOR_ON:
			framebuffer[index] |= mask;
			break;
		case SSD1306_COLOR_INVERT:
			framebuffer[index] ^= mask;
			break;
		default:
			framebuffer[index] &= (uint8_t)~mask;
			break;
	}
}

//------------------------------------------------------------------------------
bool ssd1306_get_pixel(int16_t x, int16_t y)
//------------------------------------------------------------------------------
{
	uint16_t index;
	uint8_t mask;

	if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) {
		return false;
	}

	index = (uint16_t)x + ((uint16_t)y / SSD1306_PAGE_HEIGHT) * SSD1306_WIDTH;
	mask = (uint8_t)(1U << (y % SSD1306_PAGE_HEIGHT));

	return (framebuffer[index] & mask) != 0;
}

//------------------------------------------------------------------------------
void ssd1306_draw_hline(int16_t x, int16_t y, int16_t width, ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	int16_t offset;

	for (offset = 0; offset < width; offset++) {
		ssd1306_draw_pixel(x + offset, y, color);
	}
}

//------------------------------------------------------------------------------
void ssd1306_draw_vline(int16_t x, int16_t y, int16_t height, ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	int16_t offset;

	for (offset = 0; offset < height; offset++) {
		ssd1306_draw_pixel(x, y + offset, color);
	}
}

//------------------------------------------------------------------------------
void ssd1306_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	int16_t dx = abs(x1 - x0);
	int16_t sx = x0 < x1 ? 1 : -1;
	int16_t dy = -abs(y1 - y0);
	int16_t sy = y0 < y1 ? 1 : -1;
	int16_t error = dx + dy;
	int16_t error2;

	while (true) {
		ssd1306_draw_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		error2 = 2 * error;
		if (error2 >= dy) {
			error += dy;
			x0 += sx;
		}
		if (error2 <= dx) {
			error += dx;
			y0 += sy;
		}
	}
}

//------------------------------------------------------------------------------
void ssd1306_draw_rect(int16_t x, int16_t y, int16_t width, int16_t height, ssd1306_color_t color)
//------------------------------------------------------------------------------
{
	if (width <= 0 || height <= 0) {
		return;
	}

	ssd1306_draw_hline(x, y, width, color);
	ssd1306_draw_hline(x, y + height - 1, width, color);
	ssd1306_draw_vline(x, y, height, color);
	ssd1306_draw_vline(x + width - 1, y, height, color);
}
