#include <SD.h>
#include <SoftwareSerial.h>
#include "hardware.h"
#include "buffer.h"

int Buffer::begin(const char * saveFile){
  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!SD.begin(CS)){
    Serial.println("Failed");
    return 0;
  }
  this->savePath = saveFile;
  Serial.println("OK");
  return 1;
}

int Buffer::save_measurement(measurement_t* data){
  if (this->buffer_pos == BUFFER_SIZE){
    if(!this->save_buffer_to_sd("data.bin")){
      return 0; // failed to save to sd
    }
  }
  this->measurement_buffer[this->buffer_pos].humidity = data->humidity;
  this->measurement_buffer[this->buffer_pos].temperature = data->temperature;
  this->measurement_buffer[this->buffer_pos].TVOC = data->TVOC;
  this->measurement_buffer[this->buffer_pos].eCO2 = data->eCO2;
  this->measurement_buffer[this->buffer_pos].pressure = data->pressure;

  this->buffer_pos += 1;
  return 1;
}

int Buffer::save_buffer_to_sd(const char *filepath){
  if (this->buffer_pos == 0){
    // No data to write
    return 2;
  }
  cli(); // Disables interruptions
  File dataFile = SD.open(filepath,FILE_WRITE);
  if (dataFile){
    // Writes in bin format
    dataFile.write((uint8_t *)this->measurement_buffer, BUFFER_SIZE * sizeof(measurement_t));
    dataFile.close();
    this->buffer_pos = 0;
    sei();
    return 1;
  }
  Serial.println("Error opening file");
  sei();
  return 0;
}


int Buffer::send_BT_data(SoftwareSerial &BT, int chunkSize){
  Serial.println("Saving current Buffer");
  this->save_buffer_to_sd(this->savePath);

  Serial.print("Sending data via Bluetooth: ");
  
  cli();
  File dataFile = SD.open(this->savePath,FILE_READ);
  if (dataFile){
    uint8_t buffer[chunkSize];
    while (dataFile.available()){
      int bytesRead = dataFile.read(buffer,chunkSize);
      Serial.write(buffer,bytesRead);
      BT.write(buffer, bytesRead);
      delay(10);
    }
    dataFile.close();
    Serial.println("Transfer complete");
    sei();
    return 1;
  }
  Serial.println("Failed to open file");
  sei();
  return 0;

}
