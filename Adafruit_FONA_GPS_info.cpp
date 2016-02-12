#include "Adafruit_FONA.h"

bool Adafruit_FONA::GPS_info_dir::tokenize808v1(char* gpsbuffer)
{
  // skip mode
  if (! getFirstToken(gpsbuffer)) return false;

  // skip date
  if (! getToken()) return false;

  // skip fix
  if (! getToken()) return false;

  // grab the latitude
  latp = getToken();
  if (! latp) return false;

  // grab latitude direction
  latdir = getToken();
  if (! latdir) return false;

  // grab longitude
  longp = getToken();
  if (! longp) return false;

  // grab longitude direction
  longdir = getToken();
  if (! longdir) return false;

  return true;
}

bool Adafruit_FONA::GPS_info::tokenize808v2(char* gpsbuffer)
{
  // skip GPS run status
  if (! getFirstToken(gpsbuffer)) return false;

  // skip fix status
  if (! getToken()) return false;

  // skip date
  if (! getToken()) return false;

  // grab the latitude
  latp = getToken();
  if (! latp) return false;

  // grab longitude
  longp = getToken();
  if (! longp) return false;

  return true;
}


void Adafruit_FONA::GPS_info::parseLatLong(float* lat, float* lon)
{
  *lat = atof(latp);
  *lon = atof(longp);
}


bool Adafruit_FONA::GPS_info_dir::tokenize5320(char* gpsbuffer)
{
  // grab the latitude
  latp = getFirstToken(gpsbuffer);
  if (! latp) return false;

  // grab latitude direction
  latdir = getToken();
  if (! latdir) return false;

  // grab longitude
  longp = getToken();
  if (! longp) return false;

  // grab longitude direction
  longdir = getToken();
  if (! longdir) return false;

  return true;
}


void Adafruit_FONA::GPS_info_dir::parseLatLong(float* lat, float* lon)
{
  double latitude = atof(latp);
  double longitude = atof(longp);

  // convert latitude from minutes to decimal
  float degrees = floor(latitude / 100);
  double minutes = latitude - (100 * degrees);
  minutes /= 60;
  degrees += minutes;

  // turn direction into + or -
  if (latdir[0] == 'S') degrees *= -1;

  *lat = degrees;

  // convert longitude from minutes to decimal
  degrees = floor(longitude / 100);
  minutes = longitude - (100 * degrees);
  minutes /= 60;
  degrees += minutes;

  // turn direction into + or -
  if (longdir[0] == 'W') degrees *= -1;

  *lon = degrees;
}
