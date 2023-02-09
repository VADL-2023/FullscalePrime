#include "../include/Log.h"

Log::Log(std::string flightFilename, std::string programFilename, vn::sensors::VnSensor* imu, double sTime):
mIMU(imu)
{
    //initialize variables
    this->startTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    this->sampleNumber = 1;
    this->savedParameters = false;
    this->delim = "$";

    std::string string_time = std::to_string(this->startTime);

    // catch errors involving special characters in filenames rip AAC madien flight 8/20 D:
    for (size_t i = 0; i < numSpecialCharacters; i++){
        if(flightFilename.find(specialCharacters[i]) != std::string::npos || programFilename.find(specialCharacters[i]) != std::string::npos){
            throw std::invalid_argument("Remove special characters from log filenames");
        }
    }
    
    // open flight and program data files
    std::string flight_name = flightFilename + string_time.substr(0,string_time.find(".")) + ".txt";
    std::string program_name = programFilename + string_time.substr(0,string_time.find(".")) + ".txt";
    std::cout << "Flight Data File: " << flight_name << std::endl;
    std::cout << "Program Data File: " << program_name << std::endl;
    mFlightLog.open(flight_name);
    mProgLog.open(program_name);

    if (!(mFlightLog.is_open() && mProgLog.is_open())){
        mProgLog << "Error opening file streams";
    }else {
        mFlightLog
                << "Time\t MagX\t MagY\t MagZ\t AccelX\t AccelY\t AccelZ\t Yaw\t Pitch\t Roll\t Temperature\t Pressure\t Altitude\n";
        mProgLog << "START TIME: " << std::__cxx11::to_string(startTime) << "\n";
    }
}

// deletion of Log pointer, close flight and program data files
Log::~Log() {
    mProgLog << "END TIME: ";
    mProgLog << std::__cxx11::to_string(startTime + elapsedTime()) << '\n';
    mProgLog << "END LOG";
    mFlightLog.close();
    mProgLog.close();
    mIMU = nullptr;
}

// write IMU data to flight data file, will provide calculated altitude if baseline parameters are saved
void Log::write(vn::sensors::ImuMeasurementsRegister& data){
    if (savedParameters){
        currentAlt = calcAlt(data);
    } else{
        currentAlt = -99999;
    }
    
    ++sampleNumber;
    
    char buf[256];
    sprintf(buf, "%g\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\t %6.3f\n",
            elapsedTime(), data.mag[0], data.mag[1], data.mag[2], data.accel[0], data.accel[1], data.accel[2], data.gyro[0], data.gyro[1], data.gyro[2], data.temp, data.pressure, currentAlt);
    mFlightLog << buf;
}

// write string to program data file and to console
void Log::write(std::string outputString){
    std::cout << outputString << std::endl;
    mProgLog << outputString << '\n';
}

// write string to program data file with timestamp and to console
void Log::writeTime(std::string outputString){
    std::cout << outputString << std::endl;
    mProgLog << outputString + " (" + std::to_string(sampleNumber) + " | " + std::to_string(elapsedTime()) << ")\n";
    //mProgLog << elapsedTime() << '\n';
}

// write string to program data file with special delimiters and timestamp for MATLAB postprocessing, and to console
void Log::writeDelim(std::string outputString){
    std::cout << outputString << std::endl;
    mProgLog <<delim + outputString + " (" + std::to_string(sampleNumber) + " | " + std::to_string(elapsedTime()) + ")" + delim << '\n';
}

// calculate time in milliseconds since Log pointer was created
double Log::elapsedTime(){
    currentTime = double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    return currentTime - startTime;
}

//save baseline parameters to calculate altitude
void Log::saveBaselineParameters(float Rconst, float Bconst, float pressure, float temperature, float accelg){
    this->R = Rconst;
    this->B = Bconst;
    this->pressure0 = pressure;
    this->temperature0 = temperature;
    this->accelg0 = accelg;
    savedParameters = true;
}

//calculate altitude based on baseline parameters and current IMU pressure data
float Log::calcAlt(vn::sensors::ImuMeasurementsRegister& data){
    return temperature0/B*(pow(data.pressure/pressure0,-R*B/accelg0) - 1);
}
