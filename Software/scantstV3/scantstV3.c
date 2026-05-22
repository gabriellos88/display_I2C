/**************************************************************************************
Program cycles assigns I2C slave address from 1 through to 127
communicates with OLED with each address value.
if OLED responds, it will result in display showing acknowledged address
if slave address not acknowledged, display remains blank
***************************************************************************************/
// SSD1306 OLED reset pin definition (if available)
#define SSD1306_RST       RC0_bit  // define OLED reset as D4
#define SSD1306_RST_DIR   TRISC0_bit
#define SSD1306_128_32   // defines 128x32 display works with 128x64 too, just big font.

#include "scantstV3header.h"  // include SSD1306 OLED display driver source code

unsigned int i=0;
char addr[7];
// main function
void main()
{
  OSCCON = 0x76;      // set internal oscillator to 16MHz, 0x66 = 8MHz
  ANSELC = 0;         // configure all PORTC pins as digital
  delay_ms(100);
  I2C1_Init(100000);  // initialize I2C communication with clock frequency of 100kHz
  
  //initialise SSD1306
  SSD1306_RST = 0;//sets C0 to Lo
  SSD1306_RST_DIR = 0;//sets C0 to output
  delay_ms(10);//delay
  SSD1306_RST = 1;//sets C0 to Hi
  for(i=1;i<=127;i++){//incremental address values
  IntToStr(i,addr);//converts address for display
  SSD1306_Begin(SSD1306_SWITCHCAPVCC, i);//initialises OLED with slave address
  SSD1306_ClearDisplay();//works
  SSD1306_Color = true;//through
  SSD1306_TextSize(1);//commands
  SSD1306_GotoXY(0,0);//displays
  SSD1306_Print("Addr:");//address
  SSD1306_GotoXY(20, 18);//value
  SSD1306_Print(addr);//if slave
  SSD1306_Display();//addr valid
  Delay_ms(10);
  }//ends for
}//ends void main ()