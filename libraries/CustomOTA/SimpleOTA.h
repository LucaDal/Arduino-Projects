#include "VersionCont.h"
#include "Network.h"
#include "UpdaterISPPFS.h"

typedef enum {
  NONE,
  NETWORK_BEGIN,
  NETWORK_CONNECTED,
  NETWORK_DISCONNECTED,
  SERVER_FOUND,
  FIRMWARE_DOWNLOAD_START,
  FIRMWARE_UPDATED
} SimpleOTA_State_t;

class SimpleOTA {
private:
  VersionCont *version;
  Network *network;
  FileIO *fileIO;

  unsigned long t1;

  void initVersion();
  void initNetwork();
  void initFileIO();
  void serverFirmwareCheck();
  void startDownload();
  void updateFirmware();
  friend void networkDownloadEvent(int percent);

  SimpleOTA_State_t currentState;

public:
  SimpleOTA();
  ~SimpleOTA();
  void begin();
  void loop();
};