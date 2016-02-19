#pragma once

#include <driver/gnss.h>
#include "Adafruit_FONA.h"

using namespace driver;

class SIM808_GNSS_raw : public IGNSS_raw
{
  Adafruit_FONA* fona;

  struct Context
  {
    //char buffer[120];
    Adafruit_FONA* fona;
  };
public:
  SIM808_GNSS_raw(Adafruit_FONA* fona, ...) : fona(fona) {}

  VIRTUAL MetaData getMetaData() OVERRIDE;

  VIRTUAL uint16_t getContextSize() OVERRIDE
  {
    return sizeof(Context);
  }

  VIRTUAL bool getGNSS(token_callback callback, void* context) OVERRIDE;

  // FIX: kludge call until we fully get constructor working right
  void setFONA(Adafruit_FONA* fona) { this->fona = fona; }
};




class SIM808_GNSS_raw_Factory :
  public IInstanceFactoryImpl<IGNSS_raw, SIM808_GNSS_raw, Adafruit_FONA*>
{
};

extern  SIM808_GNSS_raw_Factory   SIM808_GNSS_Factory;
