#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress sensor[3];

Adafruit_PCD8544 display = Adafruit_PCD8544(8, 10, 9);
int counter = 0;

int ccc = 0;

void setup(void)
{
  // start serial port
  //Serial.begin(9600);
  
    display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);
  display.setTextSize(2);
  display.setTextColor(BLACK);

  // Start up the library
  sensors.begin();

 // Serial.print(sensors.getDeviceCount(), DEC);
 

  // assign address manually.  the addresses below will beed to be changed
  // to valid device addresses on your bus.  device address can be retrieved
  // by using either oneWire.search(deviceAddress) or individually via
  // sensors.getAddress(deviceAddress, index)
  //insideThermometer = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
  //outsideThermometer   = { 0x28, 0x3F, 0x1C, 0x31, 0x2, 0x0, 0x0, 0x2 };

  // search for devices on the bus and assign based on an index.  ideally,
  // you would do this to initially discover addresses on the bus and then 
  // use those addresses and manually assign them (see above) once you know 
  // the devices on your bus (and assuming they don't change).
  // 
  // method 1: by index
  
  //sensors.getAddress(t1, 0);
  //sensors.getAddress(t2, 1);
  //sensors.getAddress(t3, 2);
  
  //if (!sensors.getAddress(t1, 0)) Serial.println("Unable to find address for Device 0"); 
  //if (!sensors.getAddress(t2, 1)) Serial.println("Unable to find address for Device 1"); 

  // method 2: search()
  // search() looks for the next device. Returns 1 if a new address has been
  // returned. A zero might mean that the bus is shorted, there are no devices, 
  // or you have already retrieved all of them.  It might be a good idea to 
  // check the CRC to make sure you didn't get garbage.  The order is 
  // deterministic. You will always get the same devices in the same order
  //
  // Must be called before search()
  oneWire.reset_search();
  // assigns the first address found to insideThermometer
  //if (!oneWire.search(insideThermometer)) Serial.println("Unable to find address for insideThermometer");
  // assigns the seconds address found to outsideThermometer
  //if (!oneWire.search(outsideThermometer)) Serial.println("Unable to find address for outsideThermometer");


  while ((counter < 3) && oneWire.search(sensor[counter])) {
    counter++;
  }

  // show the addresses we found on the bus
  //Serial.print("Device 0 Address: ");
  //printAddress(t1);
  //Serial.println();

  //Serial.print("Device 1 Address: ");
  //printAddress(t2);
  //Serial.println();

  // set the resolution to 9 bit
  for (int i = 0; i < counter; i++) {
    sensors.setResolution(sensor[i], TEMPERATURE_PRECISION);
  }

}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    //Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress, int ind)
{
  float tempC = sensors.getTempC(deviceAddress);
  
  char charVal[10];               //temporarily holds data from vals 
  String stringVal = "";     //data on buff is copied to this string
  
  dtostrf(tempC, 6, 3, charVal); 
  
  display.setCursor(0,ind * 16);
  display.print(charVal);
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress, int ind)
{
  printAddress(deviceAddress);
  printTemperature(deviceAddress, ind);
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  sensors.requestTemperatures();

  // print the device information
  
  display.clearDisplay();

  for (int i = 0; i < counter; i++) {
    printData(sensor[i], i);
  }

  display.setCursor(0, 2 * 16);
  display.print(ccc);
  ccc++;
  if (ccc > 99999) {
    ccc = 0;
  }

  display.display();
  
  //delay(5000);
}

