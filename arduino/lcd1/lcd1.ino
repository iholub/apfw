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
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 8);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

void setup()   {
  Serial.begin(9600);

  display.begin();
  display.setContrast(50);
  display.clearDisplay();   // clears the screen and buffer
}


void loop() {
  // text display tests
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1.5);
  display.setTextColor(BLACK);
  display.print("t1: ");
  display.println(millis());
  display.print("t2: ");
  display.println(millis());
  display.display();
  delay(2000);
  
}

