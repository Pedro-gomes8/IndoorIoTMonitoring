#include "Timer.h"
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include <Arduino.h>
#include <Wire.h>

Timer::Timer(){
  this->seconds = 8;
}

Timer::Timer(short seconds){
  this->seconds = seconds;
}

void Timer::init(short seconds){
  /*
  Sleep: seconds : 4 8
  */

  cli();

  this->seconds = seconds;


  wdt_reset();

  WDTCSR |= (1 << WDCE) | (1 << WDE);  // Enable configuration mode 

  switch(seconds){
    case 4:
      WDTCSR = (1 << WDIE) | (1 << WDP2) | (1 << WDP1);
      break;

    default: // 8  
      WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);
      break;
  }
  bitClear(TIMSK0, 0);
  sei();
}


void Timer::reset(){
  /*
    Resets the timer register
  */
    cli();
    WDTCSR |= 1 << WDIE;
    sei();
}


void Timer::deepSleep(){

  Serial.flush();
  cli();
  sleep_enable();
  Wire.end(); // Kills i2c
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sei();
  sleep_cpu(); // Waits until Controller wakes up -> See ISR in .ino file
  sleep_disable();
  Wire.begin();
}
