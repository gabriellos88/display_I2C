/**************************************************************************************
SSD1306 OLED test for PIC18F45K22 using I2C.

Connections:
  SCL/SCK -> RC3, pin 18
  SDA     -> RC4, pin 23
  RST     -> RC0, optional
***************************************************************************************/
#define SSD1306_RST       LATCbits.LATC0
#define SSD1306_RST_DIR   TRISCbits.TRISC0

#include "scantstV3header.h"  // include SSD1306 OLED display driver source code

#pragma config FOSC = HSHP
#pragma config PLLCFG = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRTEN = OFF
#pragma config BOREN = SBORDIS
#pragma config WDTEN = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = EXTMCLR
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config XINST = OFF

static void MCU_Init(void)
{
  OSCCON = 0x70;     // Internal oscillator at 16 MHz.

  ANSELA = 0x00;
  ANSELB = 0x00;
  ANSELC = 0x00;
  ANSELD = 0x00;
  ANSELE = 0x00;

  SSD1306_RST = 1;
  SSD1306_RST_DIR = 0;
}

static void SSD1306_Reset(void)
{
  SSD1306_RST = 0;
  __delay_ms(10);
  SSD1306_RST = 1;
  __delay_ms(100);
}

void main(void)
{
  MCU_Init();
  I2C1_Init(SSD1306_I2C_SPEED_HZ);
  SSD1306_Reset();

  SSD1306_Begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDR_WRITE);
  SSD1306_ClearDisplay();
  SSD1306_Color = true;
  SSD1306_TextSize(1);
  SSD1306_GotoXY(0, 0);
	
  SSD1306_Print("SSD1306 I2C");
  SSD1306_GotoXY(0, 12);
  SSD1306_Print("PIC18F45K22");
  SSD1306_GotoXY(0, 24);
  SSD1306_Print("Addr 0x3C");
  SSD1306_Display();

  while (1) {
  }
}
