#include <Servo.h>
#include <SD.h>

#define SERVO_PIN 9
#define SD_CARD_PIN 10
#define VNRRG_LEN 10
#define LED_PIN 19

// Conversions
float ft2m = 0.3048; // [m/ft]
float C2K = 273.15; // Celsius to Kelvin
float km2m = 0.001;   // [km/m]

// Constants
float R = 287;        // [kg/JK] universal gas constant
float B = 6.5*km2m;   // [K/m] variation of temperature within the troposphere

// Fixed flight parameters
float tBurn = 1.9; // [s] motor burn time
float samplingFrequency = 42; // [Hz] IMU sample rate
uint32_t imuTimeout = 40;  // [ms] length of time to wait for the IMU to respond

// Variable flight parameters
float accelRoof = 3.5; // how many g's does the program need to see in order for launch to be detected
int numDataPointsChecked4Launch = 0.4 * samplingFrequency; // how many acceleration points are averaged to see if data set is over accelRoof
int numDataPointsChecked4Apogee = 0.5 * samplingFrequency; // how many altitude points must a new max not be found for apogee to be declared
int numDataPointsChecked4Landing = 10 * samplingFrequency; // how many altitude points must a new min not be found for landing to be declared
int zThresholdForLanding = 175 * ft2m; // [m] threshold that the altitude must be within for landing
int maxFlightTime = 10 * 60; // [s] max allowable flight time, if exceeded program ends

// Calibration parameters
uint16_t numSampleReadings = 60; // amount of samples taken and averaged to find ground P and T
int imuWait = 60; //number of samples to get from IMU before actually starting to use + save data

// Initialization values
float P0;
float T0;
float g0;

// SD card file
File configFile;
File programDataFile;
File flightDataFile;

String configFileName = "config.txt";
String programDataFileName = "programData.txt";
String flightDataFileName = "flightData.txt";
int fileNum = 0;

// Servo variables
Servo theServo;
int servoStart = 72;
int servoEnd = 165;

// Structure for passing IMU data
struct imuData {
  float magX;
  float magY;
  float magZ;
  float accelX;
  float accelY;
  float accelZ;
  float yaw;
  float pitch;
  float roll;
  float temp;
  float pressure; 
  float alt;
};

void setup() {
  Serial.begin(115200);   // Communication with computer
  delay(3000);
  
  Serial3.begin(115200);  // Communication with IMU
  delay(3000);

  // Initialize SD card reading
  pinMode(SD_CARD_PIN, OUTPUT);
 
  if (!SD.begin(SD_CARD_PIN)) {
    Serial.println("SD card initialization failed");
  }

  // Initialize the servo
  theServo.attach(SERVO_PIN);
  delay(500);
  theServo.write(servoStart);
  delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK

  // Initialize the LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  delay(500);
  
  Serial.println("Initialization complete");
}

void loop() {
  // Configure IMU
  queryIMU("$VNWRG,06,0*XX\r\n");
  queryIMU("$VNWRG,82,1,0,0,0,0*XX\r\n");
  
  imuData data;

  // Get information from config file
  configFile = SD.open(configFileName.c_str());
  if(configFile.available()) {
    fileNum = configFile.parseInt();
  }
  configFile.close();

  programDataFileName = "programData" + String(fileNum) + ".txt";
  flightDataFileName = "flightData" + String(fileNum) + ".txt";

  delay(100);

  // Write header for flight data file
  flightDataFile = SD.open(flightDataFileName.c_str(), FILE_WRITE);
  if (flightDataFileName) {
    flightDataFile.println("Time\t MagX\t MagY\t MagZ\t AccelX\t AccelY\t AccelZ\t Yaw\t Pitch\t Roll\t Temperature\t Pressure\t Altitude");
  }
  flightDataFile.close();
  
  /* -------------------- P R E - F L I G H T  S T A G E -------------------- */

  bool imuConnected = true;

  // uint32_t startTime = millis();

  // Flush IMU
  writeProgramFile("IMU Flushing");
  for (int i = 0; i < imuWait; ++i) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      }
    }
  }
  writeProgramFile("IMU Flushed");

  // Calibrate ground level, pressure, temperature, and gravity
  writeProgramFile("Calibrating baseline parameters");
  float pressureSum = 0;
  float tempSum = 0;
  float gravSum = 0;
  
  for (int i = 0; i < numSampleReadings; i++) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      } else {
        pressureSum += data.pressure;
        tempSum += data.temp;
        gravSum += sqrt(pow(data.accelX,2) + pow(data.accelY,2) + pow(data.accelZ,2));
      }
    }
  }

  P0 = pressureSum / numSampleReadings;
  T0 = tempSum / numSampleReadings + C2K;
  g0 = gravSum / numSampleReadings;

  writeProgramFile("Calibrated Temperature: " + String(T0 - C2K) + " C");
  writeProgramFile("Calibrated Pressure: " + String(P0) + " kPa");
  writeProgramFile("Calibrated Gravity: " + String(g0) + " m/s^2");

  writeProgramFile("Pre-Flight Stage Completed");

  /* ------------------------ L A U N C H  S T A G E ------------------------ */

  writeProgramFile("Ready for Assembly and Launch Rail");

  float accelArray[numDataPointsChecked4Launch] = {0};
  float accelAvg = 0;
  int counter = 0;

  while (accelAvg < accelRoof * g0) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      } else {
        accelArray[counter%numDataPointsChecked4Launch] = sqrt(pow(data.accelX,2) + pow(data.accelY,2) + pow(data.accelZ,2));
        accelAvg = calcArrayAverage(accelArray, numDataPointsChecked4Launch);
        ++counter;
      }
    }
  }

  digitalWrite(LED_PIN, HIGH);
  delay(500);

  writeProgramFile("Average acceleration exceeded " + String(accelRoof * g0) + " m/s^2 over " + String(numDataPointsChecked4Launch) + " data points");
  writeProgramFile("Rocket has launched");
  writeProgramFile("Waiting for motor burn time");

  unsigned long launchTime = millis();

  /* ------------------------ M O T O R  B U R N ------------------------ */
  unsigned long tBurnEnd = tBurn * 1000 + launchTime;
  while (millis() - launchTime < tBurnEnd) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      }
    }
  }

  writeProgramFile("Motor burn time complete");

  /* --------------------- C O A S T I N G  S T A G E --------------------- */

  unsigned long maxFlightMillis = maxFlightTime * 1000;
  float zCurrent = 0;
  float maxAltitude = 0;
  int samplesSinceMaxHasChanged = 0;

  writeProgramFile("Looking for apogee");
  
  while (samplesSinceMaxHasChanged < numDataPointsChecked4Apogee && (millis() - launchTime) < maxFlightMillis) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      } else {
        zCurrent = pressure2Altitude(data.pressure);

        if (zCurrent >= maxAltitude) {
          maxAltitude = zCurrent;
          samplesSinceMaxHasChanged = 0;
        } else {
          ++samplesSinceMaxHasChanged;
        }
      }
    }
    
  }

  writeProgramFile("Altitude has not reached a new max for " + String(numDataPointsChecked4Apogee) + " samples");
  writeProgramFile("Apogee detected");

  /* ---------------------- D E S C E N T  S T A G E ---------------------- */

  float minAltitude = 1000000;
  int samplesSinceMinHasChanged = 0;

  writeProgramFile("Waiting for landing detection");

  long freqTestStartTime = millis();
  long freqTestCount = 0;

  while (!landingDetected(samplesSinceMinHasChanged, zCurrent, launchTime, maxFlightMillis)) {
    // Read the IMU if it is connected
    if (imuConnected) {
      data = readIMU();
      // Check to make sure that data was returned by the IMU
      if (isIMUEmpty(data)) {
        imuConnected = false;
      } else {
        freqTestCount++;
        zCurrent = pressure2Altitude(data.pressure);

        if (zCurrent < minAltitude) {
          minAltitude = zCurrent;
          samplesSinceMinHasChanged = 0;
        } else {
          ++samplesSinceMinHasChanged;
        }
      }
    }
    
  }

  long freqTestEndTime = millis();
  Serial.print("IMU Frequency: " + String(freqTestCount / ((freqTestEndTime - freqTestStartTime) / 1000)) + " Hz");

  writeProgramFile("Landing Detected");

  if ((samplesSinceMinHasChanged >= numDataPointsChecked4Landing) && (abs(zCurrent) < zThresholdForLanding)) {
    writeProgramFile("Altitude has not reached a new min for " + String(numDataPointsChecked4Landing) + " samples");
    writeProgramFile("Altitude was within threshold of " + String(zThresholdForLanding) + " meters");
  } else {
    writeProgramFile("Time exceeded max flight limit of " + String(maxFlightTime) + " s");
  }
  
  /* -------------------- P O S T - L A N D I N G  S T A G E -------------------- */

  // Move the servo to detach the parachute
  theServo.write(servoEnd);
  delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK

  digitalWrite(LED_PIN,HIGH);
  delay(500);

  // Update config file
  fileNum = fileNum + 1;
  configFile = SD.open(configFileName.c_str(), FILE_WRITE);
  if(configFile) {
    configFile.seek(0);
    configFile.println(String(fileNum));
  }
  configFile.close();  

  // Blink LED indefinitelys
  while(true){
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
}

// given P [kPa], returns altitude above ground level
// 0 indicates baseline measurement, R and B are constants
float pressure2Altitude(float P) {
    return T0/B*(pow(P/P0,-R*B/g0) - 1);
}

// given a float array, calculates the average of all the arrays values
float calcArrayAverage(float array[], int size) {
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum/size;
}

bool landingDetected(int samplesSinceMinHasChanged, float zCurrent, unsigned long launchTime, unsigned long maxFlightTime) {
    if ((samplesSinceMinHasChanged >= numDataPointsChecked4Landing) && (abs(zCurrent) < zThresholdForLanding)) {
        return true;
    } else if (millis() - launchTime > maxFlightTime) {
        return true;
    } else {
        return false;
    }
}

imuData readIMU() {
  imuData data;

  // FIRST QUERY
  String response = queryIMU("$VNRRG,54*XX\r\n");

  if (response.length() == 0) {
    data.magX = 0;
    data.magY = 0;
    data.magZ = 0;
    data.accelX = 0;
    data.accelY = 0;
    data.accelZ = 0;
    data.yaw = 0;
    data.pitch = 0;
    data.roll = 0;
    data.temp = 0;
    data.pressure = 0; 
    data.alt = 0;
    return data;
  }

  if (!response.substring(0,VNRRG_LEN - 1).equals("$VNRRG,54")) {
    Serial.println("ERROR: Received different response than expected from IMU");
    Serial.println("Expected: $VNRRG,54");
    Serial.println("Got: " + response.substring(0,VNRRG_LEN - 1));
  }

  char sign;
  int beginIndex;
  int commaIndex;

  // Get MagX
  beginIndex = VNRRG_LEN;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float magX = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    magX = -magX;
  }
  data.magX = magX;

  // Get MagY
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float magY = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    magY = -magY;
  }
  data.magY = magY;

  // Get MagZ
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float magZ = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    magZ = -magZ;
  }
  data.magZ = magZ;

  // Get AccelX
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float accelX = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    accelX = -accelX;
  }
  data.accelX = accelX;

  // Get AccelY
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float accelY = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    accelY = -accelY;
  }
  data.accelY = accelY;

  // Get AccelZ
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float accelZ = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    accelZ = -accelZ;
  }
  data.accelZ = accelZ;

  // Skip GyroX
  beginIndex = commaIndex + 1;
  commaIndex = response.indexOf(',', beginIndex + 1);

  // Skip GyroY
  beginIndex = commaIndex + 1;
  commaIndex = response.indexOf(',', beginIndex + 1);

  // Skip GyroZ
  beginIndex = commaIndex + 1;
  commaIndex = response.indexOf(',', beginIndex + 1);

  // Get Temp
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float temp = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    temp = -temp;
  }
  data.temp = temp;

  // Get Pressure
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float pressure = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    pressure = -pressure;
  }
  data.pressure = pressure;

  // SECOND QUERY
  response = queryIMU("$VNRRG,08*XX\r\n");

  if (response.length() == 0) {
    data.magX = 0;
    data.magY = 0;
    data.magZ = 0;
    data.accelX = 0;
    data.accelY = 0;
    data.accelZ = 0;
    data.yaw = 0;
    data.pitch = 0;
    data.roll = 0;
    data.temp = 0;
    data.pressure = 0; 
    data.alt = 0;
    return data;
  }

  if (!response.substring(0,VNRRG_LEN - 1).equals("$VNRRG,08")) {
    Serial.println("ERROR: Received different response than expected from IMU");
    Serial.println("Expected: $VNRRG,08");
    Serial.println("Got: " + response.substring(0,VNRRG_LEN - 1));
  }

  // Get MagX
  beginIndex = VNRRG_LEN;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float yaw = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    yaw = -yaw;
  }
  data.yaw = yaw;

  // Get MagY
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float pitch = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    pitch = -pitch;
  }
  data.pitch = pitch;

  // Get MagZ
  beginIndex = commaIndex + 1;
  sign = response.charAt(beginIndex);
  commaIndex = response.indexOf(',', beginIndex + 1);
  float roll = response.substring(beginIndex + 1, commaIndex).toFloat();
  if (sign == '-') {
    roll = -roll;
  }
  data.roll = roll;

  // Calculate current altitude
  float alt = -99999;
  if (T0 != 0 || P0 != 0 || g0 != 0) {
    alt = pressure2Altitude(data.pressure);
  }
  data.alt = alt;

  writeDataFile(data);
  return data;
}

String queryIMU(String request) {
  // Flush buffer if necessary
  while(Serial3.available()) {
    Serial3.read();
  }
  
  String response;
  char c;

  // Send request to IMU
  Serial3.write(request.c_str());

  uint32_t request_time = millis();

  bool valid = false;
  int attempt = 1;

  while (!valid) {
    // Get the response from the IMU
    do {
      c = Serial3.read();
      if (' ' <= c && c <= '~') {
        response += c;
      }

      // Check if a timeout has occurred
      if (millis() - request_time > imuTimeout) {
        // return an empty response
        return response;
      }

    } while (c != '*'); // Wait for the check sum
  
    // Read the check sum so it doesn't remain in the buffer
    for (int i = 0; i < 2; i = i) {
      c = Serial3.read();
      if (' ' <= c && c <= '~') {
        response += c;
        i = i + 1;
      }
    }

    if (response.substring(0,VNRRG_LEN-1).equals(request.substring(0,VNRRG_LEN-1)) || attempt++ == 2) {
      valid = true;
    } else {
      Serial.println("Invalid response from IMU");
    }
  }

  return response;
}

void writeProgramFile(String data) {
  Serial.println(data);
  programDataFile = SD.open(programDataFileName.c_str(), FILE_WRITE);
  if (programDataFile) {
    programDataFile.println(data + " (" + String(millis()) + ")");
  }
  programDataFile.close();
}

void writeDataFile(imuData data) {
  flightDataFile = SD.open(flightDataFileName.c_str(), FILE_WRITE);
  if (flightDataFileName) {
    char buf[256];
    sprintf(buf, "%lu\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f",millis(), data.magX, data.magY, data.magZ, data.accelX, data.accelY, data.accelZ, data.yaw, data.pitch, data.roll, data.temp, data.pressure, data.alt);
    flightDataFile.println(buf);
  }
  flightDataFile.close();
}

bool isIMUEmpty(imuData data) {
  return  (data.magX == 0) && (data.magY == 0) && (data.magZ == 0) &&
          (data.accelX == 0) && (data.accelY == 0) && (data.accelZ == 0) &&
          (data.yaw == 0) && (data.pitch == 0) && (data.roll == 0) &&
          (data.temp == 0) && (data.pressure == 0) && (data.alt == 0);
}
