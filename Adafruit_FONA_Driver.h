class SIM808_GNSS_raw : public IGNSS_raw
{
  struct Context
  {
    char buffer[120];
    Adafruit_FONA* fona;
  };
public:
  VIRTUAL MetaData getMetaData() OVERRIDE;

  VIRTUAL uint16_t getContextSize() OVERRIDE
  {
    return sizeof(Context);
  }

  VIRTUAL bool getGNSS(void* context, GNSS_raw_ext* data) OVERRIDE;
};




class SIM808_GNSS_raw_Factory :
  public IInstanceFactoryImpl<IGNSS_raw, SIM808_GNSS_raw>
{
};

extern  SIM808_GNSS_raw_Factory   SIM808_GNSS_Factory;
