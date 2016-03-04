#include <Arduino.h>
#include "Adafruit_FONA_Driver.h"

IDriver::MetaData Adafruit_FONA::getMetaData()
{
  static MetaData m;
  return m;
}

bool Adafruit_FONA::getGNSS(double* latitude, double* longitude,
  double* speed_kph,
  double* heading,
  double* altitude)
{
    return false;
}
