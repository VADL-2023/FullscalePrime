#include <Servo.h>
#include <SD.h>

#define SERVO_PIN 9
#define VNRRG_LEN 10

// Conversions
float ft2m = 0.3048; // [m/ft]
float C2K = 273.15; // Celsius to Kelvin
float km2m = 0.001;   // [km/m]

// Constants
float R = 287;        // [kg/JK] universal gas constant
float B = 6.5*km2m;   // [K/m] variation of temperature within the troposphere

// Fixed flight parameters
float tBurn = 1.1; // [s] motor burn time
float samplingFrequency = 30; // [Hz] IMU sample rate - TODO: need to confirm this sampling rate

// Variable flight parameters
float accelRoof = 3; // how many g's does the program need to see in order for launch to be detected
int numDataPointsChecked4Launch = 0.4 * samplingFrequency; // how many acceleration points are averaged to see if data set is over accelRoof
int numDataPointsChecked4Apogee = 0.5 * samplingFrequency; // how many altitude points must a new max not be found for apogee to be declared
int numDataPointsChecked4Landing = 10 * samplingFrequency; // how many altitude points must a new min not be found for landing to be declared
int zThresholdForLanding = 175 * ft2m; // [m] threshold that the altitude must be within for landing
int maxFlightTime = 150; // [s] max allowable flight time, if exceeded program ends
int maxParachuteDetachWaitTime = 2; // [s] maximum time to wait for the parachute detach signal to be returned from the Teensy before continuing

// Calibration parameters
uint16_t numSampleReadings = 60; // amount of samples taken and averaged to find ground P and T
int imuWait = 60; //number of samples to get from IMU before actually starting to use + save data

// Initialization values
float P0;
float T0;
float g0;

// SD card file
File myFile;

// Servo variables
Servo theServo;
int servoStart = 72;
int servoEnd = 155;

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

  // TODO: This needs to be removed
  while(!Serial.available()){}

  // Initialize SD card reading
  pinMode(10, OUTPUT);
 
//  if (!SD.begin(10)) {
//    Serial.println("initialization failed!");
//    return;
//  }

  // Initialize the servo
  theServo.attach(SERVO_PIN);
  delay(500);
  theServo.write(servoStart);
  delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK
  
  Serial.println("Initialization complete");
}

void loop() {
  // Configure IMU
  queryIMU("$VNWRG,06,0*XX\r\n");
  queryIMU("$VNWRG,82,1,0,0,0,0*XX\r\n");
  
  imuData data;
  
  /* -------------------- P R E - F L I G H T  S T A G E -------------------- */

  unsigned long startTime = millis();

  // Flush IMU
  Serial.println("IMU Flushing"); // TODO: add to log file instead
  for (int i = 0; i < imuWait; ++i) {
    readIMU();
  }
  Serial.println("IMU Flushed"); // TODO: add to log file instead

  // Calibrate ground level, pressure, temperature, and gravity
  Serial.println("Calibrating baseline parameters"); // TODO: add to log file instead
  float pressureSum = 0;
  float tempSum = 0;
  float gravSum = 0;
  
  for (int i = 0; i < numSampleReadings; i++) {
    data = readIMU();
    // TODO: save IMU data to log file
    pressureSum += data.pressure;
    tempSum += data.temp;
    gravSum += sqrt(pow(data.accelX,2) + pow(data.accelY,2) + pow(data.accelZ,2));
  }

  P0 = pressureSum / numSampleReadings;
  T0 = tempSum / numSampleReadings + C2K;
  g0 = gravSum / numSampleReadings;

  Serial.println("Calibrated Temperature: " + String(T0 - C2K) + " C"); // TODO: add to log file instead
  Serial.println("Calibrated Pressure: " + String(P0) + " kPa"); // TODO: add to log file instead
  Serial.println("Calibrated Gravity: " + String(g0) + " m/s^2"); // TODO: add to log file instead

  Serial.println("Pre-Flight Stage Completed"); // TODO: remove

  /* ------------------------ L A U N C H  S T A G E ------------------------ */

  Serial.println("Ready for Assembly and Launch Rail"); // TODO: remove

  float accelArray[numDataPointsChecked4Launch] = {0};
  float accelAvg = 0;
  int counter = 0;

  while (accelAvg < accelRoof * g0) {
    data = readIMU();
    // TOOD: save IMU data to log file
    accelArray[counter%numDataPointsChecked4Launch] = sqrt(pow(data.accelX,2) + pow(data.accelY,2) + pow(data.accelZ,2));
    accelAvg = calcArrayAverage(accelArray, numDataPointsChecked4Launch);
    ++counter;
  }

  Serial.println("Average acceleration exceeded " + String(accelRoof * g0) + " m/s^2 over " + String(numDataPointsChecked4Launch) + " data points"); // TODO: add to log file instead
  Serial.println("Rocket has launched"); // TODO: add to log file instead
  Serial.println("Waiting for motor burn time"); // TODO: add to log file instead

  unsigned long launchTime = millis();

  /* ------------------------ M O T O R  B U R N ------------------------ */
  unsigned long tBurnEnd = tBurn * 1000 + launchTime;
  while (millis() < tBurnEnd) {
    data = readIMU();
    // TODO: save IMU data to log file
  }

  Serial.println("Motor burn time complete"); // TODO: add to log file instead

  /* --------------------- C O A S T I N G  S T A G E --------------------- */

  unsigned long maxFlightMillis = maxFlightTime * 1000;
  float zCurrent = 0;
  float maxAltitude = 0;
  int samplesSinceMaxHasChanged = 0;

  Serial.println("Looking for apogee"); // TODO: add to log file instead
  
  while (samplesSinceMaxHasChanged < numDataPointsChecked4Apogee && (millis() - launchTime) < maxFlightMillis) {
    data = readIMU();
    // TODO: save IMU data to log file
    zCurrent = pressure2Altitude(data.pressure);

    if (zCurrent >= maxAltitude) {
      maxAltitude = zCurrent;
      samplesSinceMaxHasChanged = 0;
    } else {
      ++samplesSinceMaxHasChanged;
    }
  }

  Serial.println("Altitude has not reached a new max for " + String(numDataPointsChecked4Apogee) + " samples"); // TODO: add to log file instead
  Serial.println("Apogee detected"); // TODO: add to log file instead

  /* ---------------------- D E S C E N T  S T A G E ---------------------- */

  float minAltitude = 1000000;
  int samplesSinceMinHasChanged = 0;

  Serial.println("Waiting for landing detection"); // TODO: add to log file instead

  while (samplesSinceMinHasChanged < numDataPointsChecked4Landing && abs(zCurrent) < zThresholdForLanding && (millis() - launchTime) < maxFlightMillis) {
    data = readIMU();
    // TODO: save IMU data to log file
    zCurrent = pressure2Altitude(data.pressure);

    if (zCurrent < minAltitude) {
      minAltitude = zCurrent;
      samplesSinceMinHasChanged = 0;
    } else {
      ++samplesSinceMinHasChanged;
    }
  }

  if ((samplesSinceMinHasChanged >= numDataPointsChecked4Landing) && (abs(zCurrent) < zThresholdForLanding)) {
    Serial.println("Altitude has not reached a new min for " + String(numDataPointsChecked4Landing) + " samples"); // TODO: add to log file instead
    Serial.println("Altitude was within threshold of " + String(zThresholdForLanding) + " meters"); // TODO: add to log file instead
  } else {
    Serial.println("Time exceeded max flight limit of " + String(maxFlightTime) + " s"); // TODO: add to log file instead
  }
  
  Serial.println("Landing Detected"); // TODO: add to log file instead

  /* -------------------- P O S T - L A N D I N G  S T A G E -------------------- */

  // Move the servo to detach the parachute
  theServo.write(servoEnd);
  delay(500); // DON'T DELETE THIS DELAY OTHERWISE IT WON'T WORK
 
  while(true){} // Halt
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

imuData readIMU() {
  imuData data;

  // FIRST QUERY
  String response = queryIMU("$VNRRG,54*XX\r\n");

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

  bool valid = false;
  int attempt = 1;

  while (!valid) {
    // Get the response from the IMU
    do {
      c = Serial3.read();
      if (' ' <= c && c <= '~') {
        response += c;
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
    }
  }
  
  return response;
}
