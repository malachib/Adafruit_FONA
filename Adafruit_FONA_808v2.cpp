#include "Adafruit_FONA.h"

#ifdef DRIVERS
#include "alloca.h"
#include "Adafruit_FONA_Driver.h"
#include "Tokenizer.h"

IDriver::MetaData SIM808_GNSS_raw::getMetaData()
{
  MetaData md;

  md.deviceType = IDriver::DeviceType::GNSS;

  return md;
}

bool SIM808_GNSS_raw::getGNSS(IGNSS_raw_token_callback callback, void* context)
{
  static const GnssFields sequence[] =
  {
    GNSS_STATUS_RUN,
    GNSS_STATUS_FIX,
    GNSS_TIMESTAMP,
    GNSS_LATITUDE,
    GNSS_LONGITUDE,
    GNSS_ALTITUDE,
    GNSS_SPEED,
    GNSS_HEADING
  };

  fona->getReply(F("AT+CGNSINF"));

  // lifted & adapted from original FONA code
  char *p = prog_char_strstr(fona->replybuffer, (prog_char*)F("SINF"));
  if (p == 0) return false;

  p+=6;

  // do parsing here
  TokenizerInPlace tokenizer(p, ",");

  for(int i = 0; i < (sizeof(sequence) / sizeof(GnssFields)); i++)
  {
    // destructively write to original buffer , cuz I know it's safe
    // in this context - we'll be overwriting the comma.  Also
    // this call auto-advances forward
    char* token = tokenizer.parseTokenDestructive();

    // callback can abort the call early.  This is not an error, but rather
    // the consumer telling us we don't need to process anything further
    if(!callback(sequence[i], token, context))
      break;
  }

  // lifted straight from original FONA code
  fona->readline(); // eat 'OK'

  return true;
}

SIM808_GNSS_raw_Factory   SIM808_GNSS_Factory;

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
  SIM808_GNSS_raw gnssRaw;
  //GNSS_raw_ext data;
  //void* context = alloca(gnssRaw.getContextSize());
  //gnssRaw.initContext(context);
  //gnssRaw.getGNSS(context, &data);
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
