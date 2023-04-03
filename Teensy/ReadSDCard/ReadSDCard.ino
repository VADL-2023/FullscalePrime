#include <SPI.h>
#include <SdFat.h>

#define SD_CARD_PIN 10

SdFat sd;

void displayDirectoryContent(SdFile& aDirectory, byte tabulation);

void setup() {
  Serial.begin(115200);   // Communication with computer
  delay(3000);

  if (!sd.begin(SD_CARD_PIN, SPI_HALF_SPEED)) {
    Serial.println("SD card initialization failed");
  }
}

void loop() {
  SdFile dir;
  
  // Open root directory
  if (!dir.open("/")) {
    sd.errorHalt(&Serial, F("dir.open failed"));
  }
  displayDirectoryContent(dir, 0);

  while (true) {
    // Do nothing  
  }
}

void displayDirectoryContent(SdFile& aDirectory, byte tabulation) {
  SdFile file;
  char fileName[40];

  if (!aDirectory.isDir()) return;
  aDirectory.rewind();

  while (file.openNext(&aDirectory, O_READ)) {
    if (!file.isHidden()) {
      file.getName(fileName, sizeof(fileName));
      for (uint8_t i = 0; i < tabulation; i++) Serial.write('\t');
      Serial.print(fileName);

      if (file.isDir()) {
        Serial.println(F("/"));
        displayDirectoryContent(file, tabulation + 1);
      } else {
        Serial.write('\t'); Serial.print(file.fileSize()); Serial.println(F(" bytes"));
      }
    }
    file.close();
  }
}
