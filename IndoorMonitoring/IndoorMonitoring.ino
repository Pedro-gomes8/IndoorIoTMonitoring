#include <SPI.h>
#include <Wire.h>

#include <SoftwareSerial.h>

// ------- AUX LIBRARIES -----
#include "hardware.h"
#include "measurements.h"
#include "sensors.h"
#include "buffer.h"
#include "Timer.h"


// ------- CONSTANTS ------
#define SERIAL_BAUDRATE 19200
#define BLUETOOTH_BAUDRATE 9600
#define SLEEPTIME 8


// ------- INITIALIZATION ------
SoftwareSerial Bluetooth(9, 8); // RX, TX
String w;

Sensors sensors;
Buffer buffer;
Timer Sleep;

void print_measurement(measurement_t *measure)
{
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

void setup()
{
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(SERIAL_BAUDRATE);
  Bluetooth.begin(BLUETOOTH_BAUDRATE);
  while (!Serial && !Bluetooth)
  {
    ; // Wait for serial port to connect
  }
  Serial.println("Initializing Sensors ----");

  /* --------------------------------------
   *           SENSORS
   *. --------------------------------------
   */
  // delay(10);
  if (!sensors.begin())
  {
    Serial.println("Failed");
  }

  Serial.println("Initializing Buffer ----");
  if (!buffer.begin("data.bin"))
  {
    Serial.println("Failed");
  }


  pinMode(MY_LED_BUILTIN, OUTPUT);

  Sleep.init(SLEEPTIME);
}


void handleMeasurement(){
  static short cycles = 0;
    if (cycles == 7){ // Measure: Base sleep of 8 seconds -> 8x7 = 56 secs =~ 1min. Speed of 4 = 28secs.
      measurement_t measure;
      sensors.measure(&measure);
      if (!buffer.save_measurement(&measure))
      {
        Serial.println("FAILED TO SAVE");
      }
      print_measurement(&measure);
      cycles = 0;
      buffer.send_BT_data(Bluetooth, 20);
    } else cycles++;
}

ISR(WDT_vect)
{
  // Do nothing
}


void handleRequest(){
  if (Bluetooth.available())
  {
    w = Bluetooth.readString();
    cli();
    Serial.println(w);
    Serial.flush();
    sei();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(MY_LED_BUILTIN, HIGH); // sets the digital pin 13 on
  // delay(1000)                        // waits for a second

  // Check if it's time to measure.
  handleMeasurement();
  // Sleeps
  Sleep.deepSleep();
  Serial.println("Woke up");
  Serial.flush();

  // Sends HANDSHAKE availability
  Bluetooth.println("Ready"); 
  delay(1000); // Stays up for 1sec.

  // Handles request
  handleRequest();

  Sleep.reset();
}
