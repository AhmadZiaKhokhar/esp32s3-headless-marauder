#include "FS.h"
#include "SD_MMC.h"
#include <SoftwareSerial.h>
#define LINK_RX 4
#define LINK_TX 13
SoftwareSerial LinkSerial;

void setup() {
  Serial.begin(115200);
  LinkSerial.begin(38400, SWSERIAL_8N1, LINK_RX, LINK_TX);
  if(!SD_MMC.begin("/sdcard", true)){
    Serial.println("SD Card Mount Failed");
    return;
  }
  Serial.println("CAM SD Receiver Ready (1-bit mode)");
}

void handleList() {
  File root = SD_MMC.open("/");
  File file = root.openNextFile();
  while (file) {
    LinkSerial.println(String(file.name()));
    file = root.openNextFile();
  }
  LinkSerial.println("ENDLIST");
  Serial.println("Sent file list");
}

String readBlockUntil(const char* endMarker, unsigned long timeoutMs) {
  String content = "";
  unsigned long start = millis();
  bool first = true;
  while (millis() - start < timeoutMs) {
    if (LinkSerial.available()) {
      String line = LinkSerial.readStringUntil(10);
      line.trim();
      if (line == endMarker) {
        return content;
      }
      if (!first) content += "\n";
      content += line;
      first = false;
      start = millis();
    }
  }
  return content;
}

void handleAppend(String filename) {
  String content = readBlockUntil("ENDAPPEND", 5000);
  File file = SD_MMC.open("/" + filename, FILE_APPEND);
  if (file) {
    file.println(content);
    file.close();
    LinkSerial.println("OK");
    Serial.println("Appended to: " + filename);
  } else {
    LinkSerial.println("ERR");
    Serial.println("Failed to open for append: " + filename);
  }
}

void handleWrite(String filename) {
  String content = readBlockUntil("ENDWRITE", 5000);
  if (SD_MMC.exists("/" + filename)) {
    SD_MMC.remove("/" + filename);
  }
  File file = SD_MMC.open("/" + filename, FILE_WRITE);
  if (file) {
    file.print(content);
    file.close();
    LinkSerial.println("OK");
    Serial.println("Wrote: " + filename);
  } else {
    LinkSerial.println("ERR");
    Serial.println("Failed to open for write: " + filename);
  }
}

void handleRead(String filename) {
  File file = SD_MMC.open("/" + filename, FILE_READ);
  if (!file) {
    LinkSerial.println("ERR");
    Serial.println("Failed to open for read: " + filename);
    return;
  }
  while (file.available()) {
    String line = file.readStringUntil('\n');
    LinkSerial.println(line);
  }
  file.close();
  LinkSerial.println("ENDREAD");
  Serial.println("Read: " + filename);
}

void handleDelete(String filename) {
  if (SD_MMC.remove("/" + filename)) {
    LinkSerial.println("OK");
    Serial.println("Deleted: " + filename);
  } else {
    LinkSerial.println("ERR");
    Serial.println("Delete failed (or not found): " + filename);
  }
}

void loop() {
  if (LinkSerial.available()) {
    String data = LinkSerial.readStringUntil(10);
    data.trim();
    if (data.length() == 0) return;

    if (data == "LIST") {
      handleList();
    }
    else if (data.startsWith("APPEND:")) {
      handleAppend(data.substring(7));
    }
    else if (data.startsWith("WRITE:")) {
      handleWrite(data.substring(6));
    }
    else if (data.startsWith("READ:")) {
      handleRead(data.substring(5));
    }
    else if (data.startsWith("DELETE:")) {
      handleDelete(data.substring(7));
    }
    else {
      File file = SD_MMC.open("/data.txt", FILE_APPEND);
      if(file){
        file.println(data);
        file.close();
        Serial.print("Saved: ");
        Serial.println(data);
      }
    }
  }
}
