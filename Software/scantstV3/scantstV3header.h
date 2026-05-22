/******************************************************************************
oledalmdriver128x32.h
SSD1306 OLED header for mikroC PRO for PIC compiler.
Reference: Adafruit Industries SSD1306 OLED driver and graphics library.
The driver is for I2C mode only.
******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
//------------------------------ Definitions ---------------------------------//
// I2C Definitions
#define SSD1306_Start    I2C1_Start
#define SSD1306_Write    I2C1_Wr
#define SSD1306_Stop     I2C1_Stop

// Display size definitions
#define SSD1306_128_32
#define SSD1306_LCDWIDTH            128
#define SSD1306_LCDHEIGHT            32
// Command Defines
#define SSD1306_SETCONTRAST          0x81
#define SSD1306_DISPLAYALLON_RESUME  0xA4
#define SSD1306_DISPLAYALLON         0xA5
#define SSD1306_NORMALDISPLAY        0xA6
#define SSD1306_INVERTDISPLAY_       0xA7
#define SSD1306_DISPLAYOFF           0xAE
#define SSD1306_DISPLAYON            0xAF
#define SSD1306_SETDISPLAYOFFSET     0xD3
#define SSD1306_SETCOMPINS           0xDA
#define SSD1306_SETVCOMDETECT        0xDB
#define SSD1306_SETDISPLAYCLOCKDIV   0xD5
#define SSD1306_SETPRECHARGE         0xD9
#define SSD1306_SETMULTIPLEX         0xA8
#define SSD1306_SETLOWCOLUMN         0x00
#define SSD1306_SETHIGHCOLUMN        0x10
#define SSD1306_SETSTARTLINE         0x40
#define SSD1306_MEMORYMODE           0x20
#define SSD1306_COLUMNADDR           0x21
#define SSD1306_PAGEADDR             0x22
#define SSD1306_COMSCANINC           0xC0
#define SSD1306_COMSCANDEC           0xC8
#define SSD1306_SEGREMAP             0xA0
#define SSD1306_CHARGEPUMP           0x8D
#define SSD1306_EXTERNALVCC          0x01
#define SSD1306_SWITCHCAPVCC         0x02
// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL                      0x2F
#define SSD1306_DEACTIVATE_SCROLL                    0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A

extern uint8_t _i2caddr, _vccstate, x_pos = 0, y_pos = 0, text_size = 1;
extern bool wrap = true, SSD1306_Color = true;

void ssd1306_command(uint8_t c);
void SSD1306_Begin(uint8_t vccstate, uint8_t i2caddr);
void SSD1306_TextSize(uint8_t t_size);
void SSD1306_GotoXY(uint8_t x, uint8_t y);
void SSD1306_DrawPixel(uint8_t x, uint8_t y);
void SSD1306_StartScrollRight(uint8_t start, uint8_t stop);
void SSD1306_StartScrollLeft(uint8_t start, uint8_t stop);
void SSD1306_StartScrollDiagRight(uint8_t start, uint8_t stop);
void SSD1306_StartScrollDiagLeft(uint8_t start, uint8_t stop);
void SSD1306_StopScroll(void);
void SSD1306_Dim(bool dim);
void SSD1306_Display(void);
void SSD1306_ClearDisplay(void);
void SSD1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w);
void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h);
void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_FillScreen();
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r);
void SSD1306_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r);
void SSD1306_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta);
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void SSD1306_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r);
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void SSD1306_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void SSD1306_PutC(uint8_t c);
void SSD1306_Print(char *s);
void SSD1306_PutCustomC(const uint8_t *c);
void SSD1306_DrawBMP(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h);
void SSD1306_SetTextWrap(bool w);
void SSD1306_InvertDisplay(bool i);

//--------------------------------------------------------------------------//