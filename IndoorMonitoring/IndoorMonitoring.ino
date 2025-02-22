
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// ------- SENSORS -----
#include <Adafruit_BMP280.h>
#include <Adafruit_SGP30.h>   // For TVOC, ECO2, H2 and Ethanol
#include <Adafruit_Si7021.h>  // For temperature and humidity Methods: readHumidity, readTemperature
#include <SoftwareSerial.h>


// ------- FUNCTIONAL LIBRARIES ----
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

// ------- AUX LIBRARIES
#include "Timer.h"


#define CS 10
#define SLEEPTIME 8


Adafruit_Si7021 si = Adafruit_Si7021();
Adafruit_SGP30 sgp;
Adafruit_BMP280 bmp;

// ------- SD ------
File myFile;

// ------ Bluetooth
SoftwareSerial Bluetooth(9,8); // RX TX
String w;

// ------ Aux Objects
Timer Sleep;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Bluetooth.begin(9600);

  while (!Serial && !Bluetooth) {
    ;  // Wait for serial port to connect
  }

  Serial.println("Initializing Sensors ----");

  /* --------------------------------------
  *           SENSORS
  *. --------------------------------------
  */
  // --- SI7021
  Serial.print("\tSi 7021: ");

  if (!si.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true)
      ;
  }
  Serial.println("OK");

  // --- SGP30
  Serial.print("\tSGP30: ");
  if (!sgp.begin()) {
    Serial.println("Did not find SGP30 sensor!");
    while (true)
      ;
  }
  Serial.println("OK");

  // --- BMP280
  Serial.print("\tBMP280: ");
  if (!bmp.begin()) {
    Serial.println("Did not find BMP280 Sensor!");
    while (true)
      ;
  }
  Serial.println("OK");


  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS)) {
    Serial.println("Failed");
    while (1)
      ;
  }
  Serial.println("OK");


  // bmp.setSampling();


  // ---- WATCHDOG
  Sleep.init(SLEEPTIME);
 
}

ISR(WDT_vect) {
 // Do nothing
}


void loop() {
  // put your main code here, to run repeatedly:
  // Sleep.deepSleep();
  // Serial.println("Woke up");
  Serial.flush();
  // Serial.println("Ready to receive data");
  delay(2000);
  
  
  if (Bluetooth.available()) {
    w = Bluetooth.readString();
    cli();
    Serial.println(w);
    Serial.flush();
    sei();
  }
  Bluetooth.println("j");
  // Sleep.reset();
}


