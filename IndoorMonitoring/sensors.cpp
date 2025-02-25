#include "sensors.h"

int Sensors::begin(){
  this->si = Adafruit_Si7021();

  Serial.print("\tSi 7021: ");

  if (!this->si.begin()){
    Serial.println("Did not find Si7021 sensor!");
    return 0;
  }
  Serial.println("OK");

  // --- SGP30
  Serial.print("\tSGP30: ");

  if (!this->sgp.begin()){
    Serial.println("Did not find SGP30 sensor!");
    return 0;
  }
  Serial.println("OK");

  // --- BMP280
  Serial.print("\tBMP280: ");

  if (!this->bmp.begin()){
    Serial.println("Did not find BMP280 Sensor!");
    return 0;
  }
  Serial.println("OK");

  return 1;
}

int Sensors::measure(measurement_t* data){
  data->humidity = this->si.readHumidity();
  data->temperature = this->si.readTemperature();

  if (! this->sgp.IAQmeasure()) {
    // Serial.println("Measurement failed");
    return -1;
  }

  data->eCO2 = this->sgp.eCO2;

  data->pressure = this->bmp.readPressure();

  return 0;
}
