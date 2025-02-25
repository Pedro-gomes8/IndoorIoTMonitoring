
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


// SoftwareSerial Bluetooth(9,8); // RX, TX
String w;

Sensors sensors;
Buffer buffer;
Timer Sleep;

// ------- SD ------

void print_measurement(measurement_t* measure){
  Serial.print("Humidity: ");
  Serial.print(measure->humidity);
  Serial.print(", Temperature: ");
  Serial.print(measure->temperature);
  Serial.print(", TVOC: ");
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
  if(!buffer.begin("data.bin")){
    Serial.println("Failed");
  }

  Sleep.init(SLEEPTIME);

}

ISR(WDT_vect){
  // Do nothing
}



void loop() {

  measurement_t measure;
  sensors.measure(&measure);
  if(! buffer.save_measurement(&measure)){
    Serial.println("FAILED TO SAVE");
  }
  Sleep.deepSleep();
  delay(2000);
  // Serial.println("OK");
  if (buffer.buffer_pos == 3){
    buffer.send_BT_data();
  }
  // if (Bluetooth.available()) {
  //   w = Bluetooth.readString();
  //   Serial.println(w);  //PC
  //   delay(10);
  //   //commands with Serial.println(); show on pc serial monitor
  // }

  


  Sleep.reset();

}