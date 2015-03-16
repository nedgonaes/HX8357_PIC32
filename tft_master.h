/* Code rewritten from Adafruit Arduino library for the TFT
 *  by Syed Tahmid Mahbub
 * The TFT itself is Adafruit product 1480
 * Included below is the text header from the original Adafruit library
 *  followed by the code
 */

/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "plib.h"
//#include <Adafruit_GFX.h>

#define _dc         LATBbits.LATB0
#define TRIS_dc     TRISBbits.TRISB0
#define _dc_high()  {LATBSET = 1;}
#define _dc_low()   {LATBCLR = 1;}

#define _cs         LATBbits.LATB1
#define TRIS_cs     TRISBbits.TRISB1
#define _cs_high()  {LATBSET = 2;}
#define _cs_low()   {LATBCLR = 2;}

#define _rst        LATBbits.LATB2
#define TRIS_rst    TRISBbits.TRISB2
#define _rst_high() {LATBSET = 4;}
#define _rst_low()  {LATBCLR = 4;}


#define HX8357D 0xD
#define HX8357B 0xB

#define HX8357_TFTWIDTH  320
#define HX8357_TFTHEIGHT 480

#define HX8357_NOP     0x00
#define HX8357_SWRESET 0x01
#define HX8357_RDDID   0x04
#define HX8357_RDDST   0x09

#define HX8357_RDPOWMODE  0x0A
#define HX8357_RDMADCTL  0x0B
#define HX8357_RDCOLMOD  0x0C
#define HX8357_RDDIM  0x0D
#define HX8357_RDDSDR  0x0F

#define HX8357_SLPIN   0x10
#define HX8357_SLPOUT  0x11
#define HX8357B_PTLON   0x12
#define HX8357B_NORON   0x13

#define HX8357_INVOFF  0x20
#define HX8357_INVON   0x21
#define HX8357_DISPOFF 0x28
#define HX8357_DISPON  0x29

#define HX8357_CASET   0x2A
#define HX8357_PASET   0x2B
#define HX8357_RAMWR   0x2C
#define HX8357_RAMRD   0x2E

#define HX8357B_PTLAR   0x30
#define HX8357_TEON  0x35
#define HX8357_TEARLINE  0x44
#define HX8357_MADCTL  0x36
#define HX8357_COLMOD  0x3A

#define HX8357_SETOSC 0xB0
#define HX8357_SETPWR1 0xB1
#define HX8357B_SETDISPLAY 0xB2
#define HX8357_SETRGB 0xB3
#define HX8357D_SETCOM  0xB6

#define HX8357B_SETDISPMODE  0xB4
#define HX8357D_SETCYC  0xB4
#define HX8357B_SETOTP 0xB7
#define HX8357D_SETC 0xB9

#define HX8357B_SET_PANEL_DRIVING 0xC0
#define HX8357D_SETSTBA 0xC0
#define HX8357B_SETDGC  0xC1
#define HX8357B_SETID  0xC3
#define HX8357B_SETDDB  0xC4
#define HX8357B_SETDISPLAYFRAME 0xC5
#define HX8357B_GAMMASET 0xC8
#define HX8357B_SETCABC  0xC9
#define HX8357_SETPANEL  0xCC


#define HX8357B_SETPOWER 0xD0
#define HX8357B_SETVCOM 0xD1
#define HX8357B_SETPWRNORMAL 0xD2

#define HX8357B_RDID1   0xDA
#define HX8357B_RDID2   0xDB
#define HX8357B_RDID3   0xDC
#define HX8357B_RDID4   0xDD

#define HX8357D_SETGAMMA 0xE0

#define HX8357B_SETGAMMA 0xC8
#define HX8357B_SETPANELRELATED  0xE9

#define HX8357_MADCTL_MY  0x80
#define HX8357_MADCTL_MX  0x40
#define HX8357_MADCTL_MV  0x20
#define HX8357_MADCTL_ML  0x10
#define HX8357_MADCTL_RGB 0x00
#define HX8357_MADCTL_BGR 0x08
#define HX8357_MADCTL_MH  0x04

// Color definitions
#define	HX8357_BLACK   0x0000
#define	HX8357_BLUE    0x001F
#define	HX8357_RED     0xF800
#define	HX8357_GREEN   0x07E0
#define HX8357_CYAN    0x07FF
#define HX8357_MAGENTA 0xF81F
#define HX8357_YELLOW  0xFFE0
#define HX8357_WHITE   0xFFFF



#define PBCLK 40000000 // peripheral bus clock
#define SPI_freq    20000000

#define tabspace 4 // number of spaces for a tab

#define dTime_ms PBCLK/2000
#define dTime_us PBCLK/2000000

void tft_init_hw(void);
void tft_spiwrite(unsigned char c);
void tft_spiwrite8(unsigned char c);
void tft_spiwrite16(unsigned short c);
void tft_writecommand(unsigned char c);
void tft_writecommand16(unsigned short c);
void tft_writedata(unsigned char c);
void tft_writedata16(unsigned short c);
void tft_commandList(unsigned char *addr);
void tft_begin(void);
void tft_setAddrWindow(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1);
void tft_pushColor(unsigned short color);
void tft_drawPixel(short x, short y, unsigned short color);
void tft_drawFastVLine(short x, short y, short h, unsigned short color);
void tft_drawFastHLine(short x, short y, short w, unsigned short color);
void tft_fillScreen(unsigned short color);
void tft_fillRect(short x, short y, short w, short h, unsigned short color);
unsigned short tft_Color565(unsigned char r, unsigned char g, unsigned char b);
void tft_setRotation(unsigned char m);
unsigned char tft_spiread(void);
unsigned char tft_readdata(void);
unsigned char tft_readcommand8(unsigned char c);

unsigned short _width, _height;

void delay_ms(unsigned long);
void delay_us(unsigned long);