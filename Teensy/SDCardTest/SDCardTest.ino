#include <SPI.h>
#include <SdFat.h>

#define SD_CARD_PIN 10

SdFat32 sd;

File32 configFile;

String configFileName = "dintino.txt";

int fileNum = 0;

void setup() {
  Serial.begin(115200);   // Communication with computer
  delay(3000);

  Serial.println("Waiting for user input");
  while(!Serial.available()){}
  while(Serial.available()){ Serial.read(); }
  
  // Initialize SD card reading
  pinMode(SD_CARD_PIN, OUTPUT);

  if (!sd.begin(SD_CARD_PIN, SPI_HALF_SPEED)) {
    Serial.println("SD Failed");
  }
}

void loop() {
//  if (sd.begin(SD_CARD_PIN, SPI_HALF_SPEED)) {
//    if (configFile.open(configFileName.c_str(), O_READ)) {
//      int data = configFile.read();
//      String fileNumStr = "";
//      while (48 <= data && data <= 57) {
//        fileNumStr += (char)data;
//        data = configFile.read();
//      }
//      fileNum = fileNumStr.toInt();
//      Serial.println("Read from config file: " + String(fileNum));
//    } else {
//      Serial.println("ERROR: opening configFile for read failed");
//    }
//    configFile.close();
//  } else {
//    Serial.println("SD failed");
//  }
//  sd.end();
  
//  if (sd.begin(SD_CARD_PIN, SPI_HALF_SPEED)) {
//    String programDataFileName = "config.txt";
//    if (programDataFile.open(programDataFileName.c_str(), O_WRITE)) {
//      programDataFile.println("Program data will go here");
//      Serial.println("Wrote to programDataFile");
//      flightDataFile.flush();
//      programDataFile.close();
//    } else {
//      Serial.println("ERROR: opening programDataFile for write failed");
//    }
//  } else {
//    Serial.println("SD failed");
//  }
//  sd.end();
  
//  String flightDataFileName = "flightData" + String(fileNum) + ".txt";
//  if (flightDataFile.open(flightDataFileName.c_str(), O_WRONLY | O_CREAT | O_AT_END)){
//    flightDataFile.println("Flight data will go here");
//    Serial.println("Wrote to flightDataFile");
//    flightDataFile.flush();
//    flightDataFile.close();
//  } else {
//    Serial.println("ERROR: opening flightDataFile for write failed");
//  }

//  if (sd.begin(SD_CARD_PIN, SPI_HALF_SPEED)) {
//    fileNum = fileNum + 1;
    if (fileNum == 0){
      Serial.println("Creating file");
      configFile = sd.open(configFileName, FILE_WRITE);
      if (configFile) {
        configFile.print("Test");
        configFile.close();
      } else {
        Serial.println("Failed to create file");
      }
    } else {
      Serial.println("File already exists");
    }

    File32 file2 = sd.open(configFileName, FILE_READ);
    if (file2) {
      while (file2.available()) {
        Serial.write(file2.read());
      }
      file2.close();
    } else {
      Serial.println("Error opening config file");
    }
    
//    if (configFile.open(configFileName.c_str(), O_RDWR)) {
//      configFile.print(String(fileNum));
//      Serial.println("Wrote to configFile: " + String(fileNum));
////      flightDataFile.flush();
//      configFile.close();
//    } else {
//      Serial.println("ERROR: opening configFile for write failed");
//    }
//  } else {
//    Serial.println("SD failed");
//  }
//  sd.end();

//  fileNum = fileNum + 1;
//  if (configFile.open(configFileName.c_str(), O_WRITE | O_TRUNC)) {
//    configFile.print(String(fileNum));
//    Serial.println("Wrote to configFile: " + String(fileNum));
//    configFile.close();
//  } else {
//    Serial.println("ERROR: opening configFile for write failed");
//  }

  Serial.println("Waiting for user input");
  while(!Serial.available()){}
  while(Serial.available()){ Serial.read(); }
}
