#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include "PCD8544_SPI.h"

#define USE_FRAME_BUFFER

#ifdef USE_FRAME_BUFFER
PCD8544_SPI_FB lcd;
#else
PCD8544_SPI lcd;
#endif

#define SENSORS_COUNT 6
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 11

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress sensor[SENSORS_COUNT];
String addressString[SENSORS_COUNT];

int counter = 0;

void setup(void)
{
  Serial.begin(9600);
  lcd.begin(false, 0xBF, 0x04, 0x12);

  // Start up the library
  sensors.begin();

  // Must be called before search()
  oneWire.reset_search();

  while ((counter < SENSORS_COUNT) && oneWire.search(sensor[counter])) {
    addressString[counter] = addressToHexString(sensor[counter]);
    Serial.print(addressString[counter]);
    Serial.print(" ");
    Serial.println(addressString[counter].substring(8));
    counter++;
  }

  // set the temperature resolution
  for (int i = 0; i < counter; i++) {
    sensors.setResolution(sensor[i], TEMPERATURE_PRECISION);
  }

}

// function to print a device address
String addressToHexString(DeviceAddress deviceAddress)
{
  char charArr[17]; //Note there needs to be 1 extra space for this to work as snprintf null terminates.
  char* myPtr = &charArr[0]; //or just myPtr=charArr; but the former described it better.
  for (uint8_t i = 0; i < 8; i++){
    snprintf(myPtr,3,"%02x",deviceAddress[i]); //convert a byte to character string, and save 2 characters (+null) to charArr;
    myPtr += 2; //increment the pointer by two characters in charArr so that next time the null from the previous go is overwritten.
  }
  return String(charArr);
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress, int ind)
{
  float tempC = sensors.getTempC(deviceAddress);

  char charVal[6];               //temporarily holds data from vals 

  dtostrf(tempC, 5, 1, charVal); 
  char strBuf[7];
  sprintf(strBuf, "%6s", charVal);

  lcd.gotoXY(0, ind);
  lcd.print(addressString[ind].substring(8));
  lcd.print(strBuf);
  Serial.print(addressString[ind]);
  Serial.print(" ");
  Serial.println(strBuf);
  
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress, int ind)
{
  printTemperature(deviceAddress, ind);
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures();

  // print the device information

  //lcd.clear();

  for (int i = 0; i < counter; i++) {
    printData(sensor[i], i);
  }

  lcd.renderAll();

}

