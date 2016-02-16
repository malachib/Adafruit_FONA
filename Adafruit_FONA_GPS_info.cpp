#include "Adafruit_FONA.h"


const char Adafruit_FONA::GPS_info::DELIMITER[] = ",";



inline bool Adafruit_FONA::GPS_info::getTokenAndParseLegacy(double* value)
{
  if(value)
  {
    char* tok = getToken();
    if(!tok) return false;

    *value = atof(tok);
  }
  return true;
}

inline bool Adafruit_FONA::GPS_info::getTokenAndParse(double* value)
{
  char* tok = getToken();
  if(!tok)  return false;

  if(value) *value = atof(tok);

  return true;
}


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

  // grab fix status for any external interested parties
  if (! getToken(fixStatus)) return false;

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


inline void Adafruit_FONA::GPS_info::parseLatLong(double* lat, double* lon)
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


void Adafruit_FONA::GPS_info_dir::parseLatLong(double* lat, double* lon)
{
  double latitude = atof(latp);
  double longitude = atof(longp);

  // convert latitude from minutes to decimal
  double degrees = floor(latitude / 100);
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

bool Adafruit_FONA::GPS_info_dir::parse808v1(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading)
{
  if(!tokenize808v1(gpsbuffer)) return false;

  parseLatLong(lat, lon);

  if(speed_kph != NULL)
  {
    char* tok = getToken();
    if(!tok) return false;
    *speed_kph = atof(tok) * 1.852;
  }

  if(!getTokenAndParse(heading)) return false;

  return true;
}


bool Adafruit_FONA::GPS_info_dir::parse808v1_altitude(char* gpsbuffer, double* altitude)
{
  if(!getFirstToken(gpsbuffer)) return false; // skip mode
  if(!getToken()) return false; // skip lat
  if(!getToken()) return false; // skip long

  if(!getTokenAndParse(altitude)) return false; // grab altitude

  return true;
}

bool Adafruit_FONA::GPS_info::parse808v2(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading, double *altitude)
{
  if(!tokenize808v2(gpsbuffer)) return false;

  parseLatLong(lat, lon);

  if(!getTokenAndParse(altitude)) return false;
  if(!getTokenAndParse(speed_kph)) return false;
  if(!getTokenAndParse(heading)) return false;

  return true;
}

bool Adafruit_FONA::GPS_info_dir::parse5320(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading, double *altitude)
{
  if(!tokenize5320(gpsbuffer)) return false;

  // skip date & time
  getToken();
  getToken();

  if(!getTokenAndParse(altitude)) return false;
  if(!getTokenAndParse(speed_kph)) return false;
  if(!getTokenAndParse(heading)) return false;

  // Shouldn't we parse lat and long sooner, in case alt/speed/heading
  // parse wrong?
  parseLatLong(lat, lon);

  return true;
}
