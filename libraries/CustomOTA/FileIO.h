#ifndef FileIO_H_
#define FileIO_H_

//#include "SPIFFS.h"
#include "LittleFS.h" //this is for the ESP8266
//#include <mbedtls/md.h> works for the esp32 an
#include <MD5Builder.h>
class FileIO {
private:
  //mbedtls_md_context_t ctx;
  MD5Builder *md5;

public:
  static constexpr const char* TEMP_BIN_FILE = "download_firmware.bin";
  FileIO();
  void format();
  void listSPIFFS();
  fs::File openFile(String fileName, bool isReadOnly);
  void closeFile(fs::File file);
  void removeFile(String fileName);
  int getFileSize(String fileName);
  void md5Update(const unsigned char* buff, int c);
  void mdContextInit();
  //void mdContextUpdate(const unsigned char* buff, int c);
  String md5Result();
};
#endif