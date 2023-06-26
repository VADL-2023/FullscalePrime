#include "sensors.h"
#include "registers.h"
#include <fstream>

#include <chrono> 
#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <unistd.h> // sleep function


#ifndef MYPROJECT_NEWLOG_H
#define MYPROJECT_NEWLOG_H

/**
 * @file Log.h
 * @class Log
 * @brief Log class to record flight results
 * 
 */
class Log{    


private:

    /**
     * @brief Flight Data Log File relative to current directory
     * 
     */
    std::ofstream mFlightLog; 

    /**
     * @brief Program Outpu Log File relative to current directory
     * 
     */
    std::ofstream mProgLog;

    /**
     * @brief VN-100 IMU sensor (pointer to the one in the main program)
     * 
     */
    vn::sensors::VnSensor* mIMU;   

    /**
     * @brief Start time in ms
     * 
     */
    double startTime;

    /**
     * @brief Current time in ms
     * 
     */
    double currentTime;

    /**
     * @brief Current sample number
     * 
     */
    double sampleNumber;

    /**
     * @brief Baseline pressure
     * 
     */
    float pressure0;

    /**
     * @brief Baseline temperature
     * 
     */
    float temperature0;

    /**
     * @brief Baseline gravity reading
     * 
     */
    float accelg0;

    /**
     * @brief Universal Gas Constant
     * 
     */
    float R;

    /**
     * @brief Variation of temperature within the troposphere 
     * 
     */
    float B;

    /**
     * @brief Current altitude in meters
     * 
     */
    float currentAlt;

    /**
     * @brief Calculates altitude based on IMU pressure data
     * 
     * @param data The data collected from the IMU Register
     * @return float represnting the current altitude in meters
     */
    float calcAlt(vn::sensors::ImuMeasurementsRegister& data);

    /**
     * @brief Boolean indicating if we have saved the parameters yet
     * 
     */
    bool savedParameters;

    /**
     * @brief match this to number of specialCharacters (don't tell matt we hardcoded this lol)
     * 
     */
    int numSpecialCharacters = 10;

    /**
     * @brief not all of these are invalid but rather be safe than sorry
     * 
     */
    std::string specialCharacters[10] = {"/", "\\", "*", "<", ">", ",", ".", "`", "~", "|"}; 

    /**
     * @brief Character to specify when events happen in Program log ($)
     * 
     */
    std::string delim;

    /**
     * @brief Timestamp of when program is initialized
     * 
     */
    std::string timestamp_;

    /**
     * @brief String representing flight name
     * 
     */
    std::string flight_name_;

    /**
     * @brief String representing program name
     * 
     */
    std::string program_name_;
    
public:

    /**
     * @brief Construct a new Log object
     * 
     * @param flightFilename Name for flight log file
     * @param programFilename Name for program log file
     * @param imu Pointer to VN-100 sensor object
     * @param sTime Timestamp that we don't use
     */
    Log(std::string flightFilename, std::string programFilename, vn::sensors::VnSensor* imu, double sTime);

    /**
     * @brief Destroy the Log object
     * 
     */
    ~Log();

    /**
     * @brief Writes IMU data to flight log
     * 
     * @param data IMU Data
     */
    void write(vn::sensors::ImuMeasurementsRegister& data);

    /**
     * @brief Writes print statements to program log
     * 
     * @param outputString Print statements
     */
    void write(std::string outputString);

    /**
     * @brief Write string to program data file with timestamp and to console
     * 
     * @param outputString The timestamp
     */
    void writeTime(std::string outputString);

    /**
     * @brief Saves the program log file while still keeping it open to writing
     * 
     */
    void tempSaveProgLog();

    /**
     * @brief Write string to program data file with special delimiters and timestamp for MATLAB postprocessing, and to console
     * 
     * @param outputString The string written in the Log
     */
    void writeDelim(std::string outputString);

    /**
     * @brief Calculate time in milliseconds since Log pointer was created
     * 
     * @return double representing the time in ms since Log was created
     */
    double elapsedTime();

    /**
     * @brief Save baseline parameters to calculate altitude
     * 
     * @param Rconst Gas Constant
     * @param Bconst Variation of temperature within the troposphere 
     * @param pressure Baseline pressure
     * @param temperature Baseline temperature
     * @param accelg Baseline gravity reading
     */
    void saveBaselineParameters(float Rconst, float Bconst, float pressure, float temperature, float accelg);

    /**
     * @brief Get the start timestamp
     * 
     * @return string representing the timestamp
     */
    std::string getTimestamp();

};

#endif //MYPROJECT_NEWLOG_H
