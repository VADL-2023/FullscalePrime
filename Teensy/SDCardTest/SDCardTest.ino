#include "SdFat.h"
//#include <SD.h>

#define SD_CARD_PIN 10

SdFat sd;

SdFile configFile;
SdFile programDataFile;
SdFile flightDataFile;

String configFileName = "config.txt";

int fileNum = 0;

void setup() {
  Serial.begin(115200);   // Communication with computer
  delay(3000);

  Serial.println("Waiting for user input");
  while(!Serial.available()){}
  while(Serial.available()){ Serial.read(); }
  
  // Initialize SD card reading
  pinMode(SD_CARD_PIN, OUTPUT);

  uint32_t start_time = millis();
  if (!sd.begin(SD_CARD_PIN, SPI_HALF_SPEED) && millis() - start_time < 2000) {
    Serial.println("SD card initialization failed");
  }

  if (configFile.open(configFileName.c_str(), O_WRITE | O_TRUNC)) {
    configFile.print(String(fileNum));
    Serial.println("Wrote to configFile");
    configFile.close();
  }
}

void loop() {
  if (configFile.open(configFileName.c_str(), O_READ)) {
    int data = configFile.read();
    String fileNumStr = "";
    while (48 <= data && data <= 57) {
      fileNumStr += (char)data;
      data = configFile.read();
    }
    fileNum = fileNumStr.toInt();
    Serial.println("Read from config file: " + String(fileNum));
    configFile.close();
  } else {
    sd.errorHalt("Opening configFile for read failed");
  }
  
  String programDataFileName = "programData" + String(fileNum) + ".txt";
  if (programDataFile.open(programDataFileName.c_str(), O_WRITE)) {
    programDataFile.println("Program data will go here");
    Serial.println("Wrote to programDataFile");
    programDataFile.close();
  } else {
    sd.errorHalt("Opening programDataFile for write failed");
  }
  
  String flightDataFileName = "flightData" + String(fileNum) + ".txt";
  if (flightDataFile.open(flightDataFileName.c_str(), O_WRITE)) {
    flightDataFile.println("Flight data will go here");
    Serial.println("Wrote to flightDataFile");
    flightDataFile.close();
  } else {
    sd.errorHalt("Opening flightDataFile for write failed");
  }
  
  fileNum = fileNum + 1;
  if (configFile.open(configFileName.c_str(), O_WRITE | O_TRUNC)) {
    configFile.print(String(fileNum));
    Serial.println("Wrote to configFile: " + String(fileNum));
    configFile.close();
  } else {
    sd.errorHalt("Opening configFile for write failed");
  }

  Serial.println("Waiting for user input");
  while(!Serial.available()){}
  while(Serial.available()){ Serial.read(); }
}
