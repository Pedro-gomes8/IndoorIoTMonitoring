#include "hardware.h"
#include "buffer.h"

int Buffer::begin(){
  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS)){
    Serial.println("Failed");
    return 0;
  }
  Serial.println("OK");
  return 1;
}

int Buffer::save_measurement(measurement_t* data){
  if (this->buffer_pos == BUFFER_SIZE){
    if(!this->save_buffer_to_sd()){
      return 0; // failed to save to sd
    }
    this->buffer_pos = 0;
  }
  this->measurement_buffer[this->buffer_pos].humidity = data->humidity;
  this->measurement_buffer[this->buffer_pos].temperature = data->temperature;
  this->measurement_buffer[this->buffer_pos].TVOC = data->TVOC;
  this->measurement_buffer[this->buffer_pos].eCO2 = data->eCO2;
  this->measurement_buffer[this->buffer_pos].pressure = data->pressure;

  this->buffer_pos += 1;
  return 1;
}

int Buffer::save_buffer_to_sd(){
  //TODO: implement
  return 0;
}
