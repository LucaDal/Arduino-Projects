#include "FileIO.h"

FileIO::FileIO() {
  if (!LittleFS.begin()) {
    //if (!SPIFFS.begin(true)) {
    Serial.println("LittleFS initialisation failed!");
    while (1) yield();
  }
}

void FileIO::format() {
  Serial.println("LittleFS Format!");
  LittleFS.format();
}

void FileIO::listSPIFFS() {
  Serial.println(F("\r\nListing LittleFS files:"));
  static const char line[] PROGMEM = "=================================================";

  Serial.println(FPSTR(line));
  Serial.println(F("  File name                              Size"));
  Serial.println(FPSTR(line));

  fs::File root = LittleFS.open("/","r");
  if (!root) {
    Serial.println(F("Failed to open directory"));
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(F("Not a directory"));
    return;
  }

  fs::File file = root.openNextFile();
  while (file) {

    if (file.isDirectory()) {
      Serial.print("DIR : ");
      String fileName = file.name();
      Serial.print(fileName);
    } else {
      String fileName = file.name();
      Serial.print("  " + fileName);
      // File path can be 31 characters maximum in LittleFS
      int spaces = 33 - fileName.length();  // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      String fileSize = (String)file.size();
      spaces = 10 - fileSize.length();  // Tabulate nicely
      if (spaces < 1) spaces = 1;
      while (spaces--) Serial.print(" ");
      Serial.println(fileSize + " bytes");
    }

    file = root.openNextFile();
  }

  Serial.println(FPSTR(line));
  Serial.println();
}

fs::File FileIO::openFile(String fileName, bool isReadOnly) {
  if (isReadOnly) {
    return LittleFS.open(fileName, "r");
  } else {
    this->removeFile(fileName);
    return LittleFS.open(fileName, "w");
  }
}

void FileIO::closeFile(fs::File file) {
  file.close();
}

void FileIO::removeFile(String fileName) {
  if (LittleFS.exists(fileName)) {
    LittleFS.remove(fileName);
  }
}

int FileIO::getFileSize(String fileName) {
  fs::File file = LittleFS.open(fileName, "r");

  if (file) {
    int fileSize = file.size();
    file.close();
    Serial.printf("filesize: %i\n", fileSize);
    return fileSize;
  }
  Serial.println("file doesn't exists");
  return 0;
}

void FileIO::md5Update(const unsigned char* buff, int c) {
  md5->add(buff,c);
}

String FileIO::md5Result() {
  md5->calculate();
  String md5String = md5->toString();
  delete md5;
  return md5String;
}

void FileIO::mdContextInit() {
  md5 = new MD5Builder();
  md5->begin();
}

/* those function works with esp32 core library
void FileIO::mdContextInit() {
  mbedtls_md_type_t md_type = MBEDTLS_MD_MD5;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
}

void FileIO::mdContextUpdate(const unsigned char* buff, int c) {
  mbedtls_md_update(&ctx, buff, c);
}

String FileIO::md5Result() {
  byte md5Result[16];
  mbedtls_md_finish(&ctx, md5Result);
  mbedtls_md_free(&ctx);

  String checksum = "";
  for (int i = 0; i < sizeof(md5Result); i++) {
    char str[3];

    sprintf(str, "%02x", (int)md5Result[i]);
    checksum += str;
  }
  return checksum;
}
*/