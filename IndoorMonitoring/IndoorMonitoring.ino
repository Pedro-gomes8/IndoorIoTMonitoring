
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
// #define BLUETOOTH_BAUDRATE 9600
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
  // Bluetooth.begin(BLUETOOTH_BAUDRATE);
  while (!Serial){; // Wait for serial port to connect
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
  // Do nothing. Just wake up
}

void handleRequest(String &request){

  if (request.compareTo("readall") == 0){
    buffer.send_BT_data();
  }
}


void handleMeasurement(){
  static short cycles = 0;
    if (cycles == 7){ // Measure: Base sleep of 8 seconds -> 8x7 = 56 secs =~ 1min. Speed of 4 = 28secs.
      // Gets measure
      measurement_t measure;
      sensors.measure(&measure);

      if (!buffer.save_measurement(&measure))
      {
        Serial.println("FAILED TO SAVE");
      }
      print_measurement(&measure);
      cycles = 0;
    } else cycles++;
}


void loop() {

  handleMeasurement();

  Sleep.deepSleep(); // Sleep
  
  // Wakes up. it stays awake for 2 seconds to receive requests
  Serial.println("Ready");
  delay(2000);

  if (Serial.available()){
    w = Serial.readString();
    handleRequest(w);
  }
  
  // DEMO CODE
  // if (buffer.buffer_pos == 3){ 
  //   buffer.send_BT_data();
  // } 

  // if (Bluetooth.available()){
  // w = Bluetooth.readString();
  // 
  // }
  // END OF DEMO CODE



  Sleep.reset();

}