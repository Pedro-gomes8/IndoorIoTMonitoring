
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// ------- SENSORS -----
#include <Adafruit_BMP280.h>
#include <Adafruit_SGP30.h> // For TVOC, ECO2, H2 and Ethanol
#include <Adafruit_Si7021.h> // For temperature and humidity Methods: readHumidity, readTemperature

#define CS 10


Adafruit_Si7021 si = Adafruit_Si7021();
Adafruit_SGP30 sgp;
Adafruit_BMP280 bmp;

// ------- SD ------
File myFile;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial){; // Wait for serial port to connect
  }

  Serial.println("Initializing Sensors ----");

  /* --------------------------------------
  *           SENSORS
  *. --------------------------------------
  */
  // --- SI7021
  Serial.print("\tSi 7021: ");

  if (!si.begin()){
    Serial.println("Did not find Si7021 sensor!");
    while (true) ;
  }
  Serial.println("OK");

  // --- SGP30
  Serial.print("\tSGP30: ");
  if (!sgp.begin()){
    Serial.println("Did not find SGP30 sensor!");
    while (true) ;
  }
  Serial.println("OK");

  // --- BMP280
  Serial.print("\tBMP280: ");
  if (!bmp.begin()){
    Serial.println("Did not find BMP280 Sensor!");
    while (true) ; 
  }
  Serial.println("OK");


  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS)){
    Serial.println("Failed");
    while(1) ;
  }
  Serial.println("OK");


  // bmp.setSampling();





}

void loop() {
  // put your main code here, to run repeatedly:

}
