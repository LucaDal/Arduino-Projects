#include "VersionCont.h"
#include "Network.h"
#include "UpdaterISPPFS.h"


class SimpleOTA {
private:
  VersionCont *version;
  Network *network;
  FileIO *fileIO;

  unsigned long t1;

  void initVersion();
  void initNetwork(int EEPROMSize, const char * base_url);
  void initFileIO();
  void serverFirmwareCheck();
  void startDownload();
  void updateFirmware();

public:
  SimpleOTA();
  void begin(int EEPROMSize,const char * base_url);
  void checkUpdates(unsigned long seconds);
};