#ifndef BUFFER_H_
#define BUFFER_H_

#define BUFFER_SIZE 8

#include <SD.h>
#include "measurements.h"

class Buffer{
  public:
  
  const char *savePath;

  /*
   * Initializes the sd card reader
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int begin(const char* saveFile);

  /*
   * saves a measurement to the buffer and, if necessary, sends the buffer to the sd
   * @param measurement_t* data pointer to were the measure is currently saved
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int save_measurement(measurement_t* data);
  int send_BT_data(SoftwareSerial &BT, int chunkSize);

  private:
  int buffer_pos;
  measurement_t measurement_buffer[BUFFER_SIZE];

  /*
   * writes everything that's on the buffer to the sd card
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int save_buffer_to_sd(const char *filepath);


};


#endif // BUFFER_H_
