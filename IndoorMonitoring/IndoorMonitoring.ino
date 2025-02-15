
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// ------- SENSORS -----
#include <SoftwareSerial.h>

#include "hardware.h"
#include "measurements.h"
#include "sensors.h"

#define SERIAL_BAUDRATE 115200
#define BLUETOOTH_BAUDRATE 9600


SoftwareSerial mySerial(9,8); // RX, TX
String w;

Sensors sensors;

// ------- SD ------
File myFile;

void print_measurement(measurement_t* measure){
  Serial.print("Humidity: ");
  Serial.print(measure->humidity);
  Serial.print(", Temperature: ");
  Serial.print(measure->temperature);
  Serial.print(", TVOC: ");
  Serial.print(measure->TVOC);
  Serial.print(", eCO2: ");
  Serial.print(measure->eCO2);
  Serial.print(", pressure: ");
  Serial.println(measure->pressure);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUDRATE);
  mySerial.begin(BLUETOOTH_BAUDRATE);
  while (!Serial){; // Wait for serial port to connect
  }

  Serial.println("Initializing Sensors ----");

  if(!sensors.begin()){
    Serial.println("Failed");
  }

  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS)){
    Serial.println("Failed");
    while(1) ;
  }
  Serial.println("OK");


  // bmp.setSampling();

  pinMode(MY_LED_BUILTIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MY_LED_BUILTIN, HIGH); // sets the digital pin 13 on
  delay(1000);            // waits for a second

  measurement_t measure;
  sensors.measure(&measure);
  print_measurement(&measure);


  if (mySerial.available()) {
    w = mySerial.readString();
    Serial.println(w);  //PC
    delay(10);
    //commands with Serial.println(); show on pc serial monitor
  }

  mySerial.println("This is a test run");
  mySerial.println("Congratulations! Device Connected!");
  mySerial.println();


  digitalWrite(MY_LED_BUILTIN, LOW);  // sets the digital pin 13 off
  delay(2000);            // waits for a second
}
