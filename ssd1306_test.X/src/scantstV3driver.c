#include "scantstV3header.h"
uint8_t _i2caddr, _vccstate, x_pos = 0, y_pos = 0, text_size = 1;

bool wrap = true, SSD1306_Color = true;

static void I2C1_WaitIdle(void)
{
  while ((SSP1CON2 & 0x1F) || (SSP1STAT & 0x04)) {
  }
}

void I2C1_Init(uint32_t clock_hz)
{
  TRISCbits.TRISC3 = 1;      // SCL/SCK, released for MSSP open-drain operation.
  TRISCbits.TRISC4 = 1;      // SDA, released for MSSP open-drain operation.

  SSP1CON1 = 0x00;
  SSP1CON2 = 0x00;
  SSP1STAT = 0x80;           // Slew-rate disabled for 100 kHz standard mode.
  SSP1ADD = (uint8_t)((_XTAL_FREQ / (4UL * clock_hz)) - 1UL);
  PIR1bits.SSP1IF = 0;
  SSP1CON1 = 0x28;           // Enable MSSP1 in I2C master mode.
}

void I2C1_Start(void)
{
  I2C1_WaitIdle();
  PIR1bits.SSP1IF = 0;
  SSP1CON2bits.SEN = 1;
  while (!PIR1bits.SSP1IF) {
  }
  PIR1bits.SSP1IF = 0;
}

uint8_t I2C1_Write(uint8_t data)
{
  I2C1_WaitIdle();
  PIR1bits.SSP1IF = 0;
  SSP1BUF = data;
  while (!PIR1bits.SSP1IF) {
  }
  PIR1bits.SSP1IF = 0;
  return SSP1CON2bits.ACKSTAT;
}

void I2C1_Stop(void)
{
  I2C1_WaitIdle();
  PIR1bits.SSP1IF = 0;
  SSP1CON2bits.PEN = 1;
  while (!PIR1bits.SSP1IF) {
  }
  PIR1bits.SSP1IF = 0;
}

const uint8_t font[] = {
0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x5F, 0x00, 0x00,
0x00, 0x07, 0x00, 0x07, 0x00,
0x14, 0x7F, 0x14, 0x7F, 0x14,
0x24, 0x2A, 0x7F, 0x2A, 0x12,
0x23, 0x13, 0x08, 0x64, 0x62,
0x36, 0x49, 0x56, 0x20, 0x50,
0x00, 0x08, 0x07, 0x03, 0x00,
0x00, 0x1C, 0x22, 0x41, 0x00,
0x00, 0x41, 0x22, 0x1C, 0x00,
0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
0x08, 0x08, 0x3E, 0x08, 0x08,
0x00, 0x80, 0x70, 0x30, 0x00,
0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x00, 0x60, 0x60, 0x00,
0x20, 0x10, 0x08, 0x04, 0x02,
0x3E, 0x51, 0x49, 0x45, 0x3E,
0x00, 0x42, 0x7F, 0x40, 0x00,
0x72, 0x49, 0x49, 0x49, 0x46,
0x21, 0x41, 0x49, 0x4D, 0x33,
0x18, 0x14, 0x12, 0x7F, 0x10,
0x27, 0x45, 0x45, 0x45, 0x39,
0x3C, 0x4A, 0x49, 0x49, 0x31,
0x41, 0x21, 0x11, 0x09, 0x07,
0x36, 0x49, 0x49, 0x49, 0x36,
0x46, 0x49, 0x49, 0x29, 0x1E,
0x00, 0x00, 0x14, 0x00, 0x00,
0x00, 0x40, 0x34, 0x00, 0x00,
0x00, 0x08, 0x14, 0x22, 0x41,
0x14, 0x14, 0x14, 0x14, 0x14,
0x00, 0x41, 0x22, 0x14, 0x08,
0x02, 0x01, 0x59, 0x09, 0x06,
0x3E, 0x41, 0x5D, 0x59, 0x4E,
0x7C, 0x12, 0x11, 0x12, 0x7C,
0x7F, 0x49, 0x49, 0x49, 0x36,
0x3E, 0x41, 0x41, 0x41, 0x22,
0x7F, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x49, 0x49, 0x49, 0x41,
0x7F, 0x09, 0x09, 0x09, 0x01,
0x3E, 0x41, 0x41, 0x51, 0x73,
0x7F, 0x08, 0x08, 0x08, 0x7F,
0x00, 0x41, 0x7F, 0x41, 0x00,
0x20, 0x40, 0x41, 0x3F, 0x01,
0x7F, 0x08, 0x14, 0x22, 0x41,
0x7F, 0x40, 0x40, 0x40, 0x40,
0x7F, 0x02, 0x1C, 0x02, 0x7F,
0x7F, 0x04, 0x08, 0x10, 0x7F,
0x3E, 0x41, 0x41, 0x41, 0x3E,
0x7F, 0x09, 0x09, 0x09, 0x06,
0x3E, 0x41, 0x51, 0x21, 0x5E,
0x7F, 0x09, 0x19, 0x29, 0x46,
0x26, 0x49, 0x49, 0x49, 0x32,
0x03, 0x01, 0x7F, 0x01, 0x03,
0x3F, 0x40, 0x40, 0x40, 0x3F,
0x1F, 0x20, 0x40, 0x20, 0x1F,
0x3F, 0x40, 0x38, 0x40, 0x3F,
0x63, 0x14, 0x08, 0x14, 0x63,
0x03, 0x04, 0x78, 0x04, 0x03,
0x61, 0x59, 0x49, 0x4D, 0x43,
0x00, 0x7F, 0x41, 0x41, 0x41,
0x02, 0x04, 0x08, 0x10, 0x20,
0x00, 0x41, 0x41, 0x41, 0x7F,
0x04, 0x02, 0x01, 0x02, 0x04,
0x40, 0x40, 0x40, 0x40, 0x40,
0x00, 0x03, 0x07, 0x08, 0x00,
0x20, 0x54, 0x54, 0x78, 0x40,
0x7F, 0x28, 0x44, 0x44, 0x38,
0x38, 0x44, 0x44, 0x44, 0x28,
0x38, 0x44, 0x44, 0x28, 0x7F,
0x38, 0x54, 0x54, 0x54, 0x18,
0x00, 0x08, 0x7E, 0x09, 0x02,
0x18, 0xA4, 0xA4, 0x9C, 0x78,
0x7F, 0x08, 0x04, 0x04, 0x78,
0x00, 0x44, 0x7D, 0x40, 0x00,
0x20, 0x40, 0x40, 0x3D, 0x00,
0x7F, 0x10, 0x28, 0x44, 0x00,
0x00, 0x41, 0x7F, 0x40, 0x00,
0x7C, 0x04, 0x78, 0x04, 0x78,
0x7C, 0x08, 0x04, 0x04, 0x78,
0x38, 0x44, 0x44, 0x44, 0x38,
0xFC, 0x18, 0x24, 0x24, 0x18,
0x18, 0x24, 0x24, 0x18, 0xFC,
0x7C, 0x08, 0x04, 0x04, 0x08,
0x48, 0x54, 0x54, 0x54, 0x24,
0x04, 0x04, 0x3F, 0x44, 0x24,
0x3C, 0x40, 0x40, 0x20, 0x7C,
0x1C, 0x20, 0x40, 0x20, 0x1C,
0x3C, 0x40, 0x30, 0x40, 0x3C,
0x44, 0x28, 0x10, 0x28, 0x44,
0x4C, 0x90, 0x90, 0x90, 0x7C,
0x44, 0x64, 0x54, 0x4C, 0x44,
0x00, 0x08, 0x36, 0x41, 0x00,
0x00, 0x00, 0x77, 0x00, 0x00,
0x00, 0x41, 0x36, 0x08, 0x00,
0x02, 0x01, 0x02, 0x04, 0x02
};//ends Font array

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }//used in drawing lines and fills

// house image buffer? height x width dictates size of buffer
// #if.. statements in middle of buffer are ignored, full buffer always displayed?
static uint8_t buffer[SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH / 8] = {
  0,   0,   0,   0,   4,  10,  58,  62, 122, 210, 208, 208, 208, 208, 208,  48,
188,  60, 172, 172, 246, 247, 255, 110, 254, 190, 126, 126,  92, 152,  96,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
128, 128, 128, 128,   0, 129, 129, 141, 143, 136, 192,  80, 208, 200,   8,   8,
140,  12,  58,  58,  71, 130, 135,   0, 161, 208, 112, 240, 160, 166, 166, 153,
139, 142, 141, 143,  15,  27,  23,  23,  27,  15,  15, 143, 190, 187, 191, 159,
204, 140, 143, 246, 246, 246, 230, 189, 255, 177, 189, 238, 195,  76,   0,   0,
  0,   0,   0,   0,   0,   0,   0,  24,  28, 222, 126, 123,  57,  63, 125, 120,
 15,  15,   3,   3,  65, 193, 225, 193, 192, 192,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  98,  99,  29

#if (SSD1306_LCDHEIGHT * SSD1306_LCDWIDTH > 96*16)// doesn't work
// if display bigger than 96x16 (128x32) extends buffer to increase image size to suit
,
153, 192,  66, 199, 132, 132, 133, 135,   0,   0, 128,  80, 120,  81, 113, 118,
126,  63,  31,  22, 227, 253, 248, 224, 192, 193, 194, 194, 194, 195, 129,   0,
  1, 131, 131, 191, 191, 230, 234, 234, 188, 248, 241, 195,  66,  71, 129, 133,
133, 174, 255, 255, 247, 255, 255, 254, 254, 255, 251, 253, 210, 208,   0,   0,
  0,   0,   0,   0,  32,  60,  52, 247, 254, 241, 245, 239, 255, 188, 182, 254,
252, 230, 243, 233, 247, 151, 153, 255, 247, 243, 223, 221, 252, 120, 120, 248,
104, 120, 248, 112, 112,  48,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
 16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,
 16,  16,  16,  16,  16,  16,  16,  17,  17,  17,  17,  16,  16,  16,  16,  16,
 16,  16,  16,  16,  31,  31,  31,  31,  17,  17,  17,  17,  17,  17,  17,  18,
 18,  25,  25,  44,  15,  31,  63,  63, 111, 127,  58,  63, 119, 119, 255, 191,
 53,  22,  31,  11,  31,  31,  47,  63,  63, 255, 207, 255, 255,  47,  37,   0,
  0,   0,   0,   0,   0,   0,  32, 251, 255, 255, 255, 255, 255, 131, 129, 193,
227, 227,  51,  55, 229, 199,   7,  15,  11,  27,  31,  28,  56, 240,  96,  96,
 97,  97,  97,  97,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,
 96,  96,  96,  96,  96,  96, 224,  96,  96,  96,  96,  96,  96,  96,  96,  96,
 96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,
 96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,
 96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  97,
 99,  98, 102, 108, 248, 240,  96,  96,   0, 255, 255, 255, 255,   0,   0,   0
 
#if (SSD1306_LCDHEIGHT == 64)// doesn't work
// if full size display 128x64 increases buffer to full image map
,
  0,   0,   0,   0,   2,  23,  29,  33,  33, 255, 255, 255, 255, 167, 167,  58,
 24, 120, 248, 232, 121, 123, 122, 158, 144,   0, 252, 158, 159, 127,  96, 128,
 14,  10,  10,  10,  10,  10,  10,  14,   0,   0,   0,  14,  10,  10,  10,  10,
 10,  14,  14,   0,   0, 128, 255, 248, 232, 192, 192, 254, 254, 254,  34,  34,
 34,  34,  34,  34, 254,  34,  34,  34,  34,  34,  34,  34, 254, 254, 254,  50,
  0,   0,   0, 254, 130, 186,  58,  58,  98,  98,  98,  98,   2, 254,   0,   0,
  0, 254, 254, 254, 254,  34,  34,  34,  34, 254, 254,  34,  34,  34,  34,  34,
254, 254, 254, 224, 127, 255, 194, 222, 126, 255, 255, 255, 255,   0,   0,   0,
  0,   0,   0,   0,   0, 128, 128, 128,  96, 191, 255, 127, 255, 255, 225, 195,
207, 140, 137,   8,   8,   8,   8,  25,  63,  59,  11,  79,  79,  73, 233, 254,
168, 160, 160,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
 32,  32,  32,  32,  60,  63,  39,  47,  63,  63, 125,  73,   9, 225,  64,  72,
120, 120, 120, 121,  73,  65,  77,  81,  25, 121,  73, 121,  49, 113,  73,  73,
200, 200, 120, 127,  80,  81,  64,  64,  64,  80,  80, 240, 224, 127,  24, 152,
144, 201, 233, 249, 249,  56, 136,   8, 144, 241, 121, 120, 160, 248, 240, 112,
  9, 121,  63, 127, 120, 104,  77,  76, 124, 255, 255, 255, 255, 248,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  1,   1,   3,   3,   3,   6,   6,  12,  12,  24,  24,  16,  48,  32,  96,  96,
192, 192, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   2,
  2,   6,   6,   0,   0,   4,   0,   0,   2,   2,  34,  98,  98, 208, 209, 145,
145,  16,  16,  16,   8,   8,   8,   4,   6,   2,   1,   1,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0, 128, 128, 192, 192, 224, 224, 248, 231, 239, 255, 119,  67,   0,   0,
  0,   0,   0,   0,   0,   0,   0, 128, 128,   0, 192, 192, 192, 192, 192, 192,
192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192, 192,  64,  64,  64,
 64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  64,  96,
 64,  64,  65,  65,  99,  99,  98, 102, 102, 108, 108, 120, 120, 120, 112, 112,
 32,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
  1,   1,   3,   2,   6,   6,  12,  12,  24,  24,  48,  48,  48,  48,  48,  48,
 48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,  48,
 48,  48,  48,  48,  48,  48,  48,  48,  16,  16,   0,   0,   0,   0,   0,   0
#endif
#endif
};// image buffer

void ssd1306_command(uint8_t c) {
  SSD1306_Start();
  SSD1306_Write(_i2caddr);
  SSD1306_Write(0);
  SSD1306_Write(c);
  SSD1306_Stop();
}
//configures SSD1306
void SSD1306_Begin(uint8_t vccstate, uint8_t i2caddr) {
  _vccstate = vccstate;
  _i2caddr  = i2caddr;
  ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
  ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  ssd1306_command(0x80);                                  // the suggested ratio 0x80
  ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  ssd1306_command(SSD1306_LCDHEIGHT - 1);
  ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  ssd1306_command(0x0);                                   // no offset
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D

  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x10); }
  else
    { ssd1306_command(0x14); }
  ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  ssd1306_command(0x00);                                  // 0x0 act like ks0108
  ssd1306_command(SSD1306_SEGREMAP | 0x1);
  ssd1306_command(SSD1306_COMSCANDEC);

 #if defined SSD1306_128_32
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x02);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  ssd1306_command(0x8F);

#elif defined SSD1306_128_64
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x12);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x9F); }
  else
    { ssd1306_command(0xCF); }

#elif defined SSD1306_96_16
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x2);                                   //ada x12
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x10); }
  else
    { ssd1306_command(0xAF); }

#endif

  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x22); }
  else
    { ssd1306_command(0xF1); }
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel

}//ends begin function

void SSD1306_DrawPixel(uint8_t x, uint8_t y) {
  if ((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
    return;
  if (SSD1306_Color)
    buffer[x + (uint16_t)(y / 8) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
  else
    buffer[x + (uint16_t)(y / 8) * SSD1306_LCDWIDTH] &=  ~(1 << (y & 7));
}

void SSD1306_StartScrollRight(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollLeft(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollDiagRight(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306_command(0X00);
  ssd1306_command(SSD1306_LCDHEIGHT);
  ssd1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X01);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollDiagLeft(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306_command(0X00);
  ssd1306_command(SSD1306_LCDHEIGHT);
  ssd1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X01);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StopScroll(void) {
  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
}

void SSD1306_Dim(bool dim) {
  uint8_t contrast;
  if (dim)
    contrast = 0; // Dimmed display
  else {
    if (_vccstate == SSD1306_EXTERNALVCC)
      contrast = 0x9F;
    else
      contrast = 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306_command(SSD1306_SETCONTRAST);
  ssd1306_command(contrast);
}

void SSD1306_Display(void) {
  uint16_t i;
  uint8_t x;
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);   // Column start address (0 = reset)
  ssd1306_command(SSD1306_LCDWIDTH-1); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0); // Page start address (0 = reset)
  #if SSD1306_LCDHEIGHT == 64
    ssd1306_command(7); // Page end address
  #endif
  #if SSD1306_LCDHEIGHT == 32
    ssd1306_command(3); // Page end address
  #endif
  #if SSD1306_LCDHEIGHT == 16
    ssd1306_command(1); // Page end address
  #endif

  for (i = 0; i < (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT / 8); i++) {
    // send a bunch of data in one xmission
    SSD1306_Start();
    SSD1306_Write(_i2caddr);
    SSD1306_Write(0x40);
    for (x = 0; x < 16; x++) {
      SSD1306_Write(buffer[i]);
      i++;
    }
    i--;
    SSD1306_Stop();
  }
}

void SSD1306_ClearDisplay(void) {
  int16_t i;
  for (i = 0; i < (SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT / 8); i++)
    buffer[i] = 0;
}

void SSD1306_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  bool steep;
  int8_t ystep;
  uint8_t dx, dy;
  int16_t err;
  steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    ssd1306_swap(x0, y0);
    ssd1306_swap(x1, y1);
  }
  if (x0 > x1) {
    ssd1306_swap(x0, x1);
    ssd1306_swap(y0, y1);
  }
  dx = x1 - x0;
  dy = abs(y1 - y0);

  err = dx / 2;
  if (y0 < y1)
    ystep = 1;
  else
    ystep = -1;

  for (; x0 <= x1; x0++) {
    if (steep)
      SSD1306_DrawPixel(y0, x0);
    else
      SSD1306_DrawPixel(x0, y0);
    err -= dy;
    if (err < 0) {
      y0  += ystep;
      err += dx;
    }
  }
}

void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w) {
   SSD1306_DrawLine(x, y, x + w - 1, y);
}

void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h) {
  SSD1306_DrawLine(x, y, x, y + h - 1);
}

void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  int16_t i;
  for (i = x; i < x + w; i++)
    SSD1306_DrawFastVLine(i, y, h);
}

void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  SSD1306_DrawPixel(x0  , y0 + r);
  SSD1306_DrawPixel(x0  , y0 - r);
  SSD1306_DrawPixel(x0 + r, y0);
  SSD1306_DrawPixel(x0 - r, y0);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    SSD1306_DrawPixel(x0 + x, y0 + y);
    SSD1306_DrawPixel(x0 - x, y0 + y);
    SSD1306_DrawPixel(x0 + x, y0 - y);
    SSD1306_DrawPixel(x0 - x, y0 - y);
    SSD1306_DrawPixel(x0 + y, y0 + x);
    SSD1306_DrawPixel(x0 - y, y0 + x);
    SSD1306_DrawPixel(x0 + y, y0 - x);
    SSD1306_DrawPixel(x0 - y, y0 - x);
  }

}

void SSD1306_DrawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      SSD1306_DrawPixel(x0 + x, y0 + y);
      SSD1306_DrawPixel(x0 + y, y0 + x);
    }
    if (cornername & 0x2) {
      SSD1306_DrawPixel(x0 + x, y0 - y);
      SSD1306_DrawPixel(x0 + y, y0 - x);
    }
    if (cornername & 0x8) {
      SSD1306_DrawPixel(x0 - y, y0 + x);
      SSD1306_DrawPixel(x0 - x, y0 + y);
    }
    if (cornername & 0x1) {
      SSD1306_DrawPixel(x0 - y, y0 - x);
      SSD1306_DrawPixel(x0 - x, y0 - y);
    }
  }

}

void SSD1306_FillCircle(int16_t x0, int16_t y0, int16_t r) {
  SSD1306_DrawFastVLine(x0, y0 - r, 2 * r + 1);
  SSD1306_FillCircleHelper(x0, y0, r, 3, 0);
}

// Used to do circles and roundrects
void SSD1306_FillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x01) {
      SSD1306_DrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
      SSD1306_DrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
    }
    if (cornername & 0x02) {
      SSD1306_DrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
      SSD1306_DrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
    }
  }

}

// Draw a rectangle
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
  SSD1306_DrawFastHLine(x, y, w);
  SSD1306_DrawFastHLine(x, y + h - 1, w);
  SSD1306_DrawFastVLine(x, y, h);
  SSD1306_DrawFastVLine(x + w - 1, y, h);
}

// Draw a rounded rectangle
void SSD1306_DrawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) {
  // smarter version
  SSD1306_DrawFastHLine(x + r, y, w - 2 * r); // Top
  SSD1306_DrawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
  SSD1306_DrawFastVLine(x, y + r, h - 2 * r); // Left
  SSD1306_DrawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
  // draw four corners
  SSD1306_DrawCircleHelper(x + r, y + r, r, 1);
  SSD1306_DrawCircleHelper(x + w - r - 1, y + r, r, 2);
  SSD1306_DrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
  SSD1306_DrawCircleHelper(x + r, y + h - r - 1, r, 8);
}

// Fill a rounded rectangle
void SSD1306_FillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) {
  // smarter version
  SSD1306_FillRect(x + r, y, w - 2 * r, h);
  // draw four corners
  SSD1306_FillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
  SSD1306_FillCircleHelper(x + r        , y + r, r, 2, h - 2 * r - 1);
}

// Draw a triangle
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  SSD1306_DrawLine(x0, y0, x1, y1);
  SSD1306_DrawLine(x1, y1, x2, y2);
  SSD1306_DrawLine(x2, y2, x0, y0);
}

// Fill a triangle
void SSD1306_FillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  int16_t a, b, y, last,
  dx01 = x1 - x0,
  dy01 = y1 - y0,
  dx02 = x2 - x0,
  dy02 = y2 - y0,
  dx12 = x2 - x1,
  dy12 = y2 - y1;
  int32_t  sa   = 0, sb   = 0;
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    ssd1306_swap(y0, y1); ssd1306_swap(x0, x1);
  }
  if (y1 > y2) {
    ssd1306_swap(y2, y1); ssd1306_swap(x2, x1);
  }
  if (y0 > y1) {
    ssd1306_swap(y0, y1); ssd1306_swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    SSD1306_DrawFastHLine(a, y0, b - a + 1);
    return;
  }

  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1 - 1; // Skip it

  for(y = y0; y <= last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) ssd1306_swap(a, b);
    SSD1306_DrawFastHLine(a, y, b - a + 1);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y <= y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) ssd1306_swap(a, b);
    SSD1306_DrawFastHLine(a, y, b - a + 1);
  }
}

void SSD1306_FillScreen() {
  uint16_t i;
  for (i = 0; i < (SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT) / 8; i++)
    buffer[i] = 0xFF;
}

void SSD1306_SetTextWrap(bool w) {
  wrap = w;
}

// invert the display
void SSD1306_InvertDisplay(bool i) {
  if (i)
    ssd1306_command(SSD1306_INVERTDISPLAY_);
  else
    ssd1306_command(SSD1306_NORMALDISPLAY);
}

// set text size
void SSD1306_TextSize(uint8_t t_size)
{
  if(t_size < 1)
    t_size = 1;
  text_size = t_size;
}

// move cursor to position (x, y)
void SSD1306_GotoXY(uint8_t x, uint8_t y)
{
  if((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
    return;
  x_pos = x;
  y_pos = y;
}

/* print single char
    \a  Set cursor position to upper left (0, 0)
    \b  Move back one position
    \n  Go to start of current line
    \r  Go to line below
*/
void SSD1306_PutC(uint8_t c) {
  uint8_t i, j, line;

  if(c == '\a') {
    x_pos = y_pos = 0;
    return;
  }
  if( (c == '\b') && (x_pos >= text_size * 6) ) {
    x_pos -= text_size * 6;
    return;
  }
  if(c == '\r') {
    x_pos = 0;
    return;
  }
  if(c == '\n') {
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) >= SSD1306_LCDHEIGHT)
      y_pos = 0;
    return;
  }

  if((c < ' ') || (c > '~'))
    c = '?';
  for(i = 0; i < 5; i++ ) {
    line = font[(c - 32) * 5 + i];

    for(j = 0; j < 7; j++, line >>= 1) {
      if(line & 0x01)
        SSD1306_Color = true;
      else
        SSD1306_Color = false;
        if(text_size == 1) SSD1306_DrawPixel(x_pos + i, y_pos + j);
        else               SSD1306_FillRect(x_pos + (i * text_size), y_pos + (j * text_size), text_size, text_size);
    }
  }

  SSD1306_Color = false;
  SSD1306_FillRect(x_pos + (5 * text_size), y_pos, text_size, 7 * text_size);

  x_pos += text_size * 6;
  if (wrap && (x_pos + (text_size * 5)) >= SSD1306_LCDWIDTH)
  {
    x_pos = 0;
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) >= SSD1306_LCDHEIGHT)
      y_pos = 0;
  }
}

// print text
void SSD1306_Print(char *s) {
  uint8_t i = 0;
  while (s[i] != '\0'){
    if (s[i] == ' ' && x_pos == 0 && wrap)
      i++;
    else
      SSD1306_PutC(s[i++]);
  }
}

// print custom char (dimension: 7x5 pixel)
void SSD1306_PutCustomC(const uint8_t *c) {
  uint8_t i, j, line;

  for(i = 0; i < 5; i++ ) {
    line = c[i];

    for(j = 0; j < 7; j++, line >>= 1) {
      if(line & 0x01)
        SSD1306_Color = 1;
      else
        SSD1306_Color = 0;
        if(text_size == 1) SSD1306_DrawPixel(x_pos + i, y_pos + j);
        else               SSD1306_FillRect(x_pos + (i * text_size), y_pos + (j * text_size), text_size, text_size);
    }
  }

  x_pos += (text_size * 6);
  if (wrap && (x_pos + (text_size * 5)) >= SSD1306_LCDWIDTH)
  {
    x_pos = 0;
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) >= SSD1306_LCDHEIGHT)
      y_pos = 0;
  }
}

// draw BMP stored in ROM
void SSD1306_DrawBMP(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
  uint16_t j;
  uint8_t i, k;
  for (j = 0; j < h/8; j++) {
    for(i = 0; i < w; i++)   {
      for(k = 0; k < 8; k++)  {
        if( bitmap[i + j*w] & 1 << k)
          SSD1306_Color = true;
        else
          SSD1306_Color = false;
        SSD1306_DrawPixel(x + i, y + j*8 + k);
      }
    }
  }
}

// end of driver code.