#ifndef SENSORS_H_
#define SENSORS_H_
#include <Adafruit_BMP280.h>
#include <Adafruit_SGP30.h> // For TVOC, ECO2, H2 and Ethanol
#include <Adafruit_Si7021.h> // For temperature and humidity Methods: readHumidity, readTemperature
#include "measurements.h"

class Sensors{
  public:
  /*
   * makes a measurement with all sensors the class is responsible for and saves
   * it in the given data pointer
   * @param measurement_t* data  were the measurements taken are registered
   * @return errorCode returns -1 if an error occured and 0 otherwise
   */
  int measure(measurement_t * data);

  /*
   * Initializes the sensors in the class is responsible for
   * @return errorCode returns -1 if an error occured and 0 otherwise
   */
  int begin();

  private:
  Adafruit_Si7021 si;
  Adafruit_SGP30 sgp;
  Adafruit_BMP280 bmp;
};

#endif // SENSORS_H_
