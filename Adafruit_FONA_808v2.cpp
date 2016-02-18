#include "Adafruit_FONA.h"

#ifdef DRIVERS
IDriver::MetaData Adafruit_FONA::getMetaData()
{
  MetaData md;
  return md;
}

bool Adafruit_FONA::getGNSS(
  double* lat,
  double* lon,
  double* speed_kph,
  double* heading,
  double* altitude)
{
}
#endif

bool Adafruit_FONA_808v2::getGPS(
  double* lat,
  double* lon,
  double* speed_kph,
  double* heading,
  double* altitude)
{
  char gpsbuffer[120];

  // FIX: not sure why it needs an explicit qualifier here.
  uint8_t res_len = Adafruit_FONA::getGPS(32, gpsbuffer, 120);

  // Parse 808 V2 response.  See table 2-3 from here for format:
  // http://www.adafruit.com/datasheets/SIM800%20Series_GNSS_Application%20Note%20V1.00.pdf
  GPS_info gps_info;

  if(!gps_info.tokenize808v2(gpsbuffer)) return false;

  if(!gps_info.hasLock808v2()) return false;

  gps_info.parseLatLong(lat, lon);

  if(!gps_info.getTokenAndParse(altitude)) return false;
  if(!gps_info.getTokenAndParse(speed_kph)) return false;
  if(!gps_info.getTokenAndParse(heading)) return false;

  return true;
  //return gps_info.parse808v2(gpsbuffer, lat, lon, speed_kph, heading, altitude);
}
