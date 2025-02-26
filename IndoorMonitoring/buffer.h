#ifndef BUFFER_H_
#define BUFFER_H_

#define BUFFER_SIZE 4 


#include <SdFat.h>
#include "measurements.h"

class Buffer {
public:

  measurement_t measurement_buffer[BUFFER_SIZE];
  uint8_t binbuffer[18];
  const char* savePath;
  int buffer_pos;
  SdFat sd;
  SdFile dataFile;
  /*
   * Initializes the sd card reader
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int begin(char* saveFile);

  /*
   * saves a measurement to the buffer and, if necessary, sends the buffer to the sd
   * @param measurement_t* data pointer to were the measure is currently saved
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int save_measurement(measurement_t* data);

  /*
  * Access the file in the SD card in chunks and send its binary contents through the Serial
  * @return errorCode returns 0 if error
  */
  
  int send_BT_data();



private:

  /*
   * writes everything that's on the buffer to the sd card
   * @return errorCode returns 0 if an error occured and 1 otherwise
   */
  int save_buffer_to_sd(char* filepath);
};


#endif  // BUFFER_H_
