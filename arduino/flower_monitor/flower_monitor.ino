#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include "PCD8544_SPI.h"
#include <dht.h>

#define USE_FRAME_BUFFER

#ifdef USE_FRAME_BUFFER
PCD8544_SPI_FB lcd;
#else
PCD8544_SPI lcd;
#endif

#define SENSORS_COUNT 2
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

// voltage start
#define BATT_VOLT_PIN A0
#define BATT_VOLT_READ_SPEED 1000
unsigned long readBattVoltTimer;
float batteryVoltage = 0.0;
#define BATT_VOLT_R1 47000 
#define BATT_VOLT_R2 51000
const float BATT_VOLT_VD = (BATT_VOLT_R1 + BATT_VOLT_R2)/BATT_VOLT_R2;
// voltage end

static char outstr[15];

// humidity sensor start
#define DHT22_PIN 4
dht DHT;
struct
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};
double humidity;
double temperature;
// humidity sensor end

void setup(void)
{
  Serial.begin(9600);
  
  readBattVoltTimer = millis(); // Start now.
  
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
  //lcd.print(addressString[ind].substring(8));
  lcd.print("Temp 1: ");
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

  if (millis() >= readBattVoltTimer) {
    readBattVoltTimer += BATT_VOLT_READ_SPEED;
    readBatteryVoltage();
  }

  //readHumidity();

  for (int i = 0; i < counter; i++) {
    printData(sensor[i], i);
  }

  printVoltage();

  //printHumidity();
  
  lcd.renderAll();

}

// voltage functions start
void readBatteryVoltage() {
  batteryVoltage = (analogRead(BATT_VOLT_PIN) * 5.0 * BATT_VOLT_VD) / 1024.0;
}

void printVoltage() {
  //dtostrf(batteryVoltage,7, 3, outstr);
  //Serial.print("V: ");
  //Serial.println(outstr);    

  char charVal[6];               //temporarily holds data from vals 

  dtostrf(batteryVoltage, 5, 1, charVal); 
  char strBuf[7];
  sprintf(strBuf, "%6s", charVal);

  lcd.gotoXY(0, 2);
  //lcd.print(addressString[ind].substring(8));
  lcd.print("Voltage:");
  lcd.print(strBuf);
}
// voltage functions end

// humidity start
void readHumidity() {
    // READ DATA
    Serial.print("DHT22, \t");

    unsigned long start = millis();
    int chk = DHT.read22(DHT22_PIN);
    unsigned long stop = millis();

    stat.total++;
    switch (chk)
    {
    case DHTLIB_OK:
        stat.ok++;
        Serial.print("OK,\t");
        break;
    case DHTLIB_ERROR_CHECKSUM:
        stat.crc_error++;
        Serial.print("Checksum error,\t");
        break;
    case DHTLIB_ERROR_TIMEOUT:
        stat.time_out++;
        Serial.print("Time out error,\t");
        break;
    case DHTLIB_ERROR_CONNECT:
        stat.connect++;
        Serial.print("Connect error,\t");
        break;
    case DHTLIB_ERROR_ACK_L:
        stat.ack_l++;
        Serial.print("Ack Low error,\t");
        break;
    case DHTLIB_ERROR_ACK_H:
        stat.ack_h++;
        Serial.print("Ack High error,\t");
        break;
    default:
        stat.unknown++;
        Serial.print("Unknown error,\t");
        break;
    }
    // DISPLAY DATA
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.print(stop - start);
    Serial.println();

  humidity = DHT.humidity;
  temperature = DHT.temperature;
}

void printHumidity() {
  char charVal[6];               //temporarily holds data from vals 
  char strBuf[7];

  dtostrf(humidity, 5, 1, charVal); 
  sprintf(strBuf, "%6s", charVal);
  lcd.gotoXY(0, 3);
  //lcd.print(addressString[ind].substring(8));
  lcd.print("Humidity");
  lcd.print(strBuf);
  
  dtostrf(temperature, 5, 1, charVal); 
  sprintf(strBuf, "%6s", charVal);
  lcd.gotoXY(0, 4);
  //lcd.print(addressString[ind].substring(8));
  lcd.print("HT:");
  lcd.print(strBuf);
  
}
// humidity end
