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
float samplingFrequency = 60; // [Hz] IMU sample rate - TODO: need to confirm this sampling rate

// Variable flight parameters
float accelRoof = 3; // how many g's does the program need to see in order for launch to be detected
int numDataPointsChecked4Launch = 8; // how many acceleration points are averaged to see if data set is over accelRoof
int numDataPointsChecked4Apogee = 10; // how many altitude points must a new max not be found for apogee to be declared
int numSecondsNoNewMinimum = 10; // [s] number of seconds to wait for no new minimum to determine landing
int numDataPointsChecked4Landing = numSecondsNoNewMinimum*samplingFrequency; // how many altitude points must a new min not be found for landing to be declared
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

  while(!Serial.available()){}
  
  Serial.println("VectorNav Teensy Test");
}

void loop() {
  // Configure IMU
  queryIMU("$VNWRG,06,0*XX\r\n");
  queryIMU("$VNWRG,82,1,0,0,0,0*XX\r\n");
  
  imuData data;
  
  /* -------------------- P R E - F L I G H T  S T A G E -------------------- */

  double startTime = millis();

  // Flush IMU
  Serial.println("IMU Flushing"); // TODO: remove
  for (int i = 0; i < imuWait; ++i) {
    readIMU();
  }
  Serial.println("IMU Flushed"); // TODO: remove

  // Calibrate ground level, pressure, temperature, and gravity
  Serial.println("Calibrating baseline parameters"); // TODO: remove
  float pressureSum = 0;
  float tempSum = 0;
  float gravSum = 0;
  
  for (int i = 0; i < numSampleReadings; i++) {
    data = readIMU();
    pressureSum += data.pressure;
    tempSum += data.temp;
    gravSum += sqrt(pow(data.accelX,2) + pow(data.accelY,2) + pow(data.accelZ,2));
  }

  P0 = pressureSum / numSampleReadings;
  T0 = tempSum / numSampleReadings + C2K;
  g0 = gravSum / numSampleReadings;

  Serial.println("Calibrated Temperature: " + String(T0 - C2K) + " C"); // TODO: remove
  Serial.println("Calibrated Pressure: " + String(P0) + " kPa"); // TODO: remove
  Serial.println("Calibrated Gravity: " + String(g0) + " m/s^2"); // TODO: remove

  Serial.println("Pre-Flight Stage Completed"); // TODO: remove

  /* ------------------------ L A U N C H  S T A G E ------------------------ */

  delay(10000);
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
    alt = T0/B*(pow(data.pressure/P0,-R*B/g0) - 1);
  }
  data.alt = alt;
  
  return data;
}

String queryIMU(String request) {
  String response;
  char c;

  // Send request to IMU
  Serial3.write(request.c_str());

  bool valid = false;

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
  }
  
  return response;
}
