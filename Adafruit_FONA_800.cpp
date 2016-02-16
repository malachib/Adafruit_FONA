#include "Adafruit_FONA.h"

bool Adafruit_FONA_800::getGPS(
  double* lat,
  double* lon,
  double* speed_kph,
  double* heading,
  double* altitude)
{
  char gpsbuffer[120];

  // we need at least a 2D fix
  if (GPSstatus() < 2)
    return false;

  // grab the mode 2^5 gps csv from the sim808
  //uint8_t res_len = getGPS_helper(32, gpsbuffer, 120);
  uint8_t res_len = Adafruit_FONA::getGPS(32, gpsbuffer, 120);

  // make sure we have a response
  if (res_len == 0)
    return false;

  // Parse 808 V1 response.
  GPS_info_dir gps_info;

  if(!gps_info.parse808v1(gpsbuffer, lat, lon, speed_kph, heading))
    return false;

  // no need to continue
  if (altitude == NULL)
    return true;

  // we need at least a 3D fix for altitude
  if (GPSstatus() < 3)
    return false;

  // grab the mode 0 gps csv from the sim808
  res_len = Adafruit_FONA::getGPS(0, gpsbuffer, 120);

  // make sure we have a response
  if (res_len == 0)
    return false;

  return gps_info.parse808v1_altitude(gpsbuffer, altitude);

}
