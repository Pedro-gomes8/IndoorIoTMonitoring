
#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ------- SENSORS -----

#include "hardware.h"
#include "measurements.h"
#include "sensors.h"
#include "buffer.h"
// --- Watchdog
#include "Timer.h"


#define SERIAL_BAUDRATE 115200
#define BLUETOOTH_BAUDRATE 9600
#define SLEEPTIME 8


SoftwareSerial Bluetooth(9,8); // RX, TX
String w;

Sensors sensors;
Buffer buffer;
Timer Sleep;

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
  Bluetooth.begin(BLUETOOTH_BAUDRATE);
  while (!Serial && !Bluetooth){; // Wait for serial port to connect
  }

  Serial.println("Initializing Sensors ----");

  if(!sensors.begin()){
    Serial.println("Failed");
  }


  Serial.println("Initializing Buffer ----");
  if(!buffer.begin()){
    Serial.println("Failed");
  }

  Sleep.init(SLEEPTIME);
  // pinMode(MY_LED_BUILTIN,OUTPUT);
}

ISR(WDT_vect){
  // Do nothing
}



void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(MY_LED_BUILTIN, HIGH); // sets the digital pin 13 on
  // delay(1000);            // waits for a second

  measurement_t measure;
  sensors.measure(&measure);
  if(! buffer.save_measurement(&measure)){
    Serial.println("FAILED TO SAVE");
  }
  print_measurement(&measure);

  Sleep.deepSleep();
  delay(2000);

  if (Bluetooth.available()) {
    w = Bluetooth.readString();
    Serial.println(w);  //PC
    delay(10);
    //commands with Serial.println(); show on pc serial monitor
  }

  Bluetooth.println("This is a test run");
  Bluetooth.println("Congratulations! Device Connected!");
  Bluetooth.println();


  digitalWrite(MY_LED_BUILTIN, LOW);  // sets the digital pin 13 off
  delay(2000);            // waits for a second
  Sleep.reset();
}
