#include "sensors.h"
#include "registers.h"
#include <fstream>

#include <chrono> 
#include <iostream>
#include <sys/time.h>
#include <ctime>

#ifndef MYPROJECT_NEWLOG_H
#define MYPROJECT_NEWLOG_H

class Log{    


private:
    std::ofstream mFlightLog; // Flight Data Log File, relative to current directory

    std::ofstream mProgLog; // Program Output Log File, relative to current directory

    vn::sensors::VnSensor* mIMU;   
     
    double startTime;
    double currentTime;
    double sampleNumber;
    float pressure0;
    float temperature0;
    float accelg0;
    float R;
    float B;
    float currentAlt;
    float calcAlt(vn::sensors::ImuMeasurementsRegister& data);
    bool savedParameters;
    int numSpecialCharacters = 10; // match this to number of specialCharacters (don't tell matt we hardcoded this lol)
    std::string specialCharacters[10] = {"/", "\\", "*", "<", ">", ",", ".", "`", "~", "|"}; // not all of these are invalid but rather be safe than sorry
    std::string delim;
    
public:

    Log(std::string flightFilename, std::string programFilename, vn::sensors::VnSensor* imu, double sTime);

    ~Log();

    void write(vn::sensors::ImuMeasurementsRegister& data);
    
    void write(std::string outputString);

    void writeTime(std::string outputString);
    
    void writeDelim(std::string outputString);
    
    double elapsedTime();
    
    void saveBaselineParameters(float Rconst, float Bconst, float pressure, float temperature, float accelg);

};

#endif //MYPROJECT_NEWLOG_H
