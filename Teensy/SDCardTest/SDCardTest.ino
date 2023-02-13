#include <SD.h>

#define SD_CARD_PIN 10

File configFile;
File programDataFile;
File flightDataFile;

String configFileName = "config.txt";

int fileNum = 0;

void setup() {
  Serial.begin(115200);   // Communication with computer
  delay(3000);
  while(!Serial.available()){}
  
  // Initialize SD card reading
  pinMode(SD_CARD_PIN, OUTPUT);
  
  if (!SD.begin(SD_CARD_PIN)) {
    Serial.println("SD card initialization failed");
  }
}

void loop() {
  configFile = SD.open(configFileName.c_str());
  while(configFile.available()) {
    fileNum = configFile.parseInt();
    Serial.println("Read from config file: " + String(fileNum));
  }
  configFile.close();
  
  String programDataFileName = "programData" + String(fileNum) + ".txt";
  programDataFile = SD.open(programDataFileName.c_str(), FILE_WRITE);
  if (programDataFile) {
    programDataFile.println("Program data will go here");
    Serial.println("Wrote to programDataFile");
  }
  programDataFile.close();

  String flightDataFileName = "flightData" + String(fileNum) + ".txt";
  flightDataFile = SD.open(flightDataFileName.c_str(), FILE_WRITE);
  if (flightDataFileName) {
    flightDataFile.println("Flight data will go here");
    Serial.println("Wrote to flightDataFile");
  }
  flightDataFile.close();

  fileNum = fileNum + 1;
  configFile = SD.open(configFileName.c_str(), FILE_WRITE);
  if(configFile) {
    configFile.seek(0);
    configFile.println(String(fileNum));
    Serial.println("Wrote to configFile");
  }
  configFile.close();  

  while (true) {}
}
