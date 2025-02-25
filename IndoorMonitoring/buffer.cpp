#include <SdFat.h>
#include <SoftwareSerial.h>
#include "hardware.h"
#include "buffer.h"

int Buffer::begin(char * saveFile){
  // --- SD CARD
  Serial.print("Initializing SD card: ");
  if (!sd.begin(CS, SPI_HALF_SPEED)){
    Serial.println("Failed");
    return 0;
  }
  this->savePath = saveFile;
  Serial.println("OK");
  return 1;
}

int Buffer::save_measurement(measurement_t* data){
  volatile static int id = 0;
  if ((this->buffer_pos == BUFFER_SIZE) && (!this->save_buffer_to_sd("data.bin"))){
    return 0; // failed to save to sd
  }
  this->measurement_buffer[this->buffer_pos].id = id;
  this->measurement_buffer[this->buffer_pos].humidity = data->humidity;
  this->measurement_buffer[this->buffer_pos].temperature = data->temperature;
  this->measurement_buffer[this->buffer_pos].eCO2 = data->eCO2;
  this->measurement_buffer[this->buffer_pos].pressure = data->pressure;

  this->buffer_pos += 1;
  id++;
  return 1;
}

int Buffer::save_buffer_to_sd(char *filePath){
  if (this->buffer_pos == 0){
    // No data to write
    return 2;
  }
  cli(); // Disables interruptions
  if (dataFile.open(filePath, O_WRONLY | O_CREAT | O_APPEND)){
    dataFile.write((uint8_t *)&(this->measurement_buffer), this->buffer_pos * sizeof(measurement_t));
    dataFile.close();
    this->buffer_pos = 0;
    sei();
    return 1;
  }
  Serial.println("Error opening file");
  sei();
  return 0;
}



int Buffer::send_BT_data(){
  Serial.println("Saving current Buffer");
  this->save_buffer_to_sd(this->savePath);

  Serial.println("Sending data: ");
  
  cli();

  if (dataFile.open(this->savePath,O_RDONLY)){
    while (dataFile.available()){
      int bytesRead = dataFile.read(this->binbuffer,18);
      Serial.write(this->binbuffer,bytesRead);
      Serial.println();
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
