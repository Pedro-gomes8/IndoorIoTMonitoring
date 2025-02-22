
#include <SPI.h>
#include <Wire.h>

// ------- SENSORS -----
#include <SoftwareSerial.h>

#include "hardware.h"
#include "measurements.h"
#include "sensors.h"
#include "buffer.h"
#include "Timer.h"
#define SERIAL_BAUDRATE 115200
#define BLUETOOTH_BAUDRATE 9600
#define CS 10
#define SLEEPTIME 8

// ------- FUNCTIONAL LIBRARIES ----
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

SoftwareSerial Bluetooth(9, 8); // RX, TX
String w;

Sensors sensors;
Buffer buffer;
Timer Sleep;

// ------- SD ------
File myFile;

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
  Serial.begin(115200);
  while (!Serial)
  {
    ; // Wait for serial port to connect
  }

  Serial.println("Initializing Sensors ----");

  /* --------------------------------------
   *           SENSORS
   *. --------------------------------------
   */
  if (!sensors.begin())
  {
    Serial.println("Failed");
  }

  Serial.println("Initializing Buffer ----");
  if (!buffer.begin())
  {
    Serial.println("Failed");
  }

  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS))
  {
    Serial.println("Failed");
    while (1)
      ;
  }

  pinMode(MY_LED_BUILTIN, OUTPUT);

  Sleep.init(SLEEPTIME);
}

ISR(WDT_vect)
{
  // Do nothing
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(MY_LED_BUILTIN, HIGH); // sets the digital pin 13 on
  delay(1000);                        // waits for a second

  measurement_t measure;
  sensors.measure(&measure);
  if (!buffer.save_measurement(&measure))
  {
    Serial.println("FAILED TO SAVE");
  }
  print_measurement(&measure);

  // Sleep.deepSleep();
  // Serial.println("Woke up");
  Serial.flush();
  // Serial.println("Ready to receive data");
  delay(2000);

  if (Bluetooth.available())
  {
    w = Bluetooth.readString();
    cli();
    Serial.println(w);
    Serial.flush();
    sei();
  }
  Bluetooth.println("j");
  // Sleep.reset();
}
