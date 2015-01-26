/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
 Adafruit_PCD8544 display = Adafruit_PCD8544(8, 10, 9);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

int c = 0;

void setup()   {
  Serial.begin(9600);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.setTextSize(2);
  display.setTextColor(BLACK);


}


void loop() {
  
  float floatVal=123.234;
  char charVal[10];               //temporarily holds data from vals 
  String stringVal = "";     //data on buff is copied to this string
  
  floatVal=23.29;
  dtostrf(floatVal, 4, 1, charVal); 
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("t1:");
  display.print(charVal);
  
  floatVal=123.296;
  dtostrf(floatVal, 4, 1, charVal); 
  
  
  display.setCursor(0,16);
  display.print("t2:");
  display.print(charVal);
  
  
  display.setCursor(0,32);
  display.print(charVal);
  display.print(999 - c);
  display.display();
  delay(100);
  
    c++;
  if (c > 999) {
    c = 0;
  }

}



