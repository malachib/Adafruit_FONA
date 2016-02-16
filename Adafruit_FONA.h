/***************************************************
  This is a library for our Adafruit FONA Cellular Module

  Designed specifically to work with the Adafruit FONA
  ----> http://www.adafruit.com/products/1946
  ----> http://www.adafruit.com/products/1963

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#ifndef ADAFRUIT_FONA_H
#define ADAFRUIT_FONA_H

#include "includes/FONAConfig.h"
#include "includes/FONAExtIncludes.h"
#include "includes/platform/FONAPlatform.h"



#define FONA800L 1
#define FONA800H 6

#define FONA808_V1 2
#define FONA808_V2 3

#define FONA3G_A 4
#define FONA3G_E 5

// Uncomment to changed the preferred SMS storage
//#define FONA_PREF_SMS_STORAGE "SM"

#define FONA_HEADSETAUDIO 0
#define FONA_EXTAUDIO 1

#define FONA_STTONE_DIALTONE 1
#define FONA_STTONE_BUSY 2
#define FONA_STTONE_CONGESTION 3
#define FONA_STTONE_PATHACK 4
#define FONA_STTONE_DROPPED 5
#define FONA_STTONE_ERROR 6
#define FONA_STTONE_CALLWAIT 7
#define FONA_STTONE_RINGING 8
#define FONA_STTONE_BEEP 16
#define FONA_STTONE_POSTONE 17
#define FONA_STTONE_ERRTONE 18
#define FONA_STTONE_INDIANDIALTONE 19
#define FONA_STTONE_USADIALTONE 20

#define FONA_DEFAULT_TIMEOUT_MS 500

#define FONA_HTTP_GET   0
#define FONA_HTTP_POST  1
#define FONA_HTTP_HEAD  2

#define FONA_CALL_READY 0
#define FONA_CALL_FAILED 1
#define FONA_CALL_UNKNOWN 2
#define FONA_CALL_RINGING 3
#define FONA_CALL_INPROGRESS 4

// full operation mode
#define FONA_SLEEPMODE_AWAKE 0
// can still receive paging/SMS but serial port is shut down
// To wake up: Pull DTR pin low or "receive a voice or data call"
// be advised: not all boards break out the DTR pin!
#define FONA_SLEEPMODE_SLEEP 1
// can still receive paging/SMS and serial port will auto-wake
// but first character user -> FONA will be lost
#define FONA_SLEEPMODE_STANDBY 2

class ISIMCOM_Driver
{
public:
  class Context
  {

  };

  virtual bool getGPS(
    Context* context,
    float* lat,
    float* lon,
    float* speed_kph = 0,
    float* heading = 0,
    float* alt = 0) = 0;

  virtual uint8_t getGPSStatus(Context* context) = 0;
};

class SIMCOM_808v1_Driver : public ISIMCOM_Driver {};
class SIMCOM_808v2_Driver : public ISIMCOM_Driver {};
class SIMCOM_5320_Driver : public ISIMCOM_Driver {};


class Adafruit_FONA : public FONAStreamType {
 public:
  Adafruit_FONA(int8_t r);
  boolean begin(FONAStreamType &port);
  uint8_t type();

  // Stream
  int available(void);
  size_t write(uint8_t x);
  int read(void);
  int peek(void);
  void flush();

  // FONA 3G requirements
  boolean setBaudrate(uint16_t baud);

  // RTC
  boolean enableRTC(uint8_t i);
  boolean readRTC(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *hr, uint8_t *min, uint8_t *sec);

  // Battery and ADC
  boolean getADCVoltage(uint16_t *v);
  boolean getBattPercent(uint16_t *p);
  boolean getBattVoltage(uint16_t *v);

  // sleep functions (generally use DTR to wake up)
  bool setSleepMode(uint8_t sleep);

  // SIM query
  uint8_t unlockSIM(char *pin);
  uint8_t getSIMCCID(char *ccid);
  uint8_t getNetworkStatus(void);
  uint8_t getRSSI(void);

  // IMEI
  uint8_t getIMEI(char *imei);

  // set Audio output
  boolean setAudio(uint8_t a);
  boolean setVolume(uint8_t i);
  uint8_t getVolume(void);
  boolean playToolkitTone(uint8_t t, uint16_t len);
  boolean setMicVolume(uint8_t a, uint8_t level);
  boolean playDTMF(char tone);

  // FM radio functions.
  boolean tuneFMradio(uint16_t station);
  boolean FMradio(boolean onoff, uint8_t a = FONA_HEADSETAUDIO);
  boolean setFMVolume(uint8_t i);
  int8_t getFMVolume();
  int8_t getFMSignalLevel(uint16_t station);

  // SMS handling
  boolean setSMSInterrupt(uint8_t i);
  uint8_t getSMSInterrupt(void);
  int8_t getNumSMS(void);
  boolean readSMS(uint8_t i, char *smsbuff, uint16_t max, uint16_t *readsize);
  boolean sendSMS(char *smsaddr, char *smsmsg);
  boolean deleteSMS(uint8_t i);
  boolean getSMSSender(uint8_t i, char *sender, int senderlen);
  boolean sendUSSD(char *ussdmsg, char *ussdbuff, uint16_t maxlen, uint16_t *readlen);

  // Time
  boolean enableNetworkTimeSync(boolean onoff);
  boolean enableNTPTimeSync(boolean onoff, FONAFlashStringPtr ntpserver=0);
  boolean getTime(char *buff, uint16_t maxlen);

  // GPRS handling
  boolean enableGPRS(boolean onoff);
  uint8_t GPRSstate(void);
  boolean getGSMLoc(uint16_t *replycode, char *buff, uint16_t maxlen);
  boolean getGSMLoc(float *lat, float *lon);
  void setGPRSNetworkSettings(FONAFlashStringPtr apn, FONAFlashStringPtr username=0, FONAFlashStringPtr password=0);

  // GPS handling
  boolean enableGPS(boolean onoff);
  int8_t GPSstatus(void);
  uint8_t getGPS(uint8_t arg, char *buffer, uint8_t maxbuff);
  boolean getGPS(float *lat, float *lon, float *speed_kph=0, float *heading=0, float *altitude=0);
  boolean getGPS_new(double *lat, double *lon, double *speed_kph=0, double *heading=0, double *altitude=0);
  boolean enableGPSNMEA(uint8_t nmea);

  // TCP raw connections
  boolean TCPconnect(char *server, uint16_t port);
  boolean TCPclose(void);
  boolean TCPconnected(void);
  boolean TCPsend(char *packet, uint8_t len);
  uint16_t TCPavailable(void);
  uint16_t TCPread(uint8_t *buff, uint8_t len);

  // HTTP low level interface (maps directly to SIM800 commands).
  boolean HTTP_init();
  boolean HTTP_term();
  void HTTP_para_start(FONAFlashStringPtr parameter, boolean quoted = true);
  boolean HTTP_para_end(boolean quoted = true);
  boolean HTTP_para(FONAFlashStringPtr parameter, const char *value);
  boolean HTTP_para(FONAFlashStringPtr parameter, FONAFlashStringPtr value);
  boolean HTTP_para(FONAFlashStringPtr parameter, int32_t value);
  boolean HTTP_data(uint32_t size, uint32_t maxTime=10000);
  boolean HTTP_action(uint8_t method, uint16_t *status, uint16_t *datalen, int32_t timeout = 10000);
  boolean HTTP_readall(uint16_t *datalen);
  boolean HTTP_ssl(boolean onoff);

  // HTTP high level interface (easier to use, less flexible).
  boolean HTTP_GET_start(char *url, uint16_t *status, uint16_t *datalen);
  void HTTP_GET_end(void);
  boolean HTTP_POST_start(char *url, FONAFlashStringPtr contenttype, const uint8_t *postdata, uint16_t postdatalen,  uint16_t *status, uint16_t *datalen);
  void HTTP_POST_end(void);
  void setUserAgent(FONAFlashStringPtr useragent);

  // HTTPS
  void setHTTPSRedirect(boolean onoff);

  // PWM (buzzer)
  boolean setPWM(uint16_t period, uint8_t duty = 50);

  // Phone calls
  boolean callPhone(char *phonenum);
  uint8_t getCallStatus(void);
  boolean hangUp(void);
  boolean pickUp(void);
  boolean callerIdNotification(boolean enable, uint8_t interrupt = 0);
  boolean incomingCallNumber(char* phonenum);

  // Helper functions to verify responses.
  boolean expectReply(FONAFlashStringPtr reply, uint16_t timeout = 10000);
  boolean sendCheckReply(char *send, char *reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FONAFlashStringPtr send, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(char* send, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);

  // consolidate tokenization & parsing behavior
  struct GPS_info
  {
    const static char DELIMITER[];

    char* fixStatus;
    char* latp;
    char* longp;

    static char* getToken() { return strtok(NULL, DELIMITER); }
    static char* getToken(char*& token) { token = getToken(); return token; }
    static char* getFirstToken(char* gpsbuffer)
    { return strtok(gpsbuffer, DELIMITER); }

    // Seems doing things this way could result in improper values if one
    // decides to skip tokens
    static bool getTokenAndParseLegacy(double* value);
    static bool getTokenAndParse(double* value);

    bool tokenize808v2(char* gpsbuffer);

    inline void parseLatLong(double* lat, double* lon);
    bool parse808v2(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading, double *altitude);
    uint8_t parseFixStatus() { return atoi(fixStatus); }
    bool hasLock808v2() { return fixStatus[0] == '1'; }
  };


  struct GPS_info_dir : GPS_info
  {
    char* latdir;
    char* longdir;

    bool tokenize808v1(char* gpsbuffer);
    bool tokenize5320(char* gpsbuffer);

    void parseLatLong(double* lat, double* lon);

    bool parse5320(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading, double *altitude);
    bool parse808v1(char* gpsbuffer, double *lat, double *lon, double *speed_kph, double *heading);
    bool parse808v1_altitude(char* gpsbuffer, double* altitude);
  };

 protected:
  int8_t _rstpin;
  uint8_t _type;

  char replybuffer[255];
  FONAFlashStringPtr apn;
  FONAFlashStringPtr apnusername;
  FONAFlashStringPtr apnpassword;
  boolean httpsredirect;
  FONAFlashStringPtr useragent;
  FONAFlashStringPtr ok_reply;

  // HTTP helpers
  boolean HTTP_setup(char *url);

  void flushInput();
  uint16_t readRaw(uint16_t b);
  uint8_t readline(uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS, boolean multiline = false);
  uint8_t getReply(char *send, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FONAFlashStringPtr send, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FONAFlashStringPtr prefix, char *suffix, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FONAFlashStringPtr prefix, int32_t suffix, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  uint8_t getReply(FONAFlashStringPtr prefix, int32_t suffix1, int32_t suffix2, uint16_t timeout); // Don't set default value or else function call is ambiguous.
  uint8_t getReplyQuoted(FONAFlashStringPtr prefix, FONAFlashStringPtr suffix, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);

  boolean sendCheckReply(FONAFlashStringPtr prefix, char *suffix, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FONAFlashStringPtr prefix, int32_t suffix, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  boolean sendCheckReply(FONAFlashStringPtr prefix, int32_t suffix, int32_t suffix2, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);
  boolean sendCheckReplyQuoted(FONAFlashStringPtr prefix, FONAFlashStringPtr suffix, FONAFlashStringPtr reply, uint16_t timeout = FONA_DEFAULT_TIMEOUT_MS);


  boolean parseReply(FONAFlashStringPtr toreply,
          uint16_t *v, char divider  = ',', uint8_t index=0);
  boolean parseReply(FONAFlashStringPtr toreply,
          char *v, char divider  = ',', uint8_t index=0);
  boolean parseReplyQuoted(FONAFlashStringPtr toreply,
          char *v, int maxlen, char divider, uint8_t index);

  boolean sendParseReply(FONAFlashStringPtr tosend,
       FONAFlashStringPtr toreply,
       uint16_t *v, char divider = ',', uint8_t index=0);

  static boolean _incomingCall;
  static void onIncomingCall();

  FONAStreamType *mySerial;
};

class Adafruit_FONA_808v2 : public Adafruit_FONA {
public:
  Adafruit_FONA_808v2 (int8_t r) : Adafruit_FONA(r) { _type = FONA808_V2; }

protected:
  bool getGPS(double* lat, double* lon, double* speed_kph=0, double* heading=0, double* altitude=0);
};


class Adafruit_FONA_3G : public Adafruit_FONA {

 public:
  Adafruit_FONA_3G (int8_t r) : Adafruit_FONA(r) { _type = FONA3G_A; }

    boolean getBattVoltage(uint16_t *v);
    boolean playToolkitTone(uint8_t t, uint16_t len);
    boolean hangUp(void);
    boolean pickUp(void);
    boolean enableGPRS(boolean onoff);
    boolean enableGPS(boolean onoff);

 protected:
    boolean parseReply(FONAFlashStringPtr toreply,
		       float *f, char divider, uint8_t index);

    boolean sendParseReply(FONAFlashStringPtr tosend,
			   FONAFlashStringPtr toreply,
			   float *f, char divider = ',', uint8_t index=0);
};

#endif
