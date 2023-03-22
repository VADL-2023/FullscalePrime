#include "SerialObject.h"
#include <chrono>

// returns current time in milliseconds since epoch
double getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

int main() {
    SerialObject vector_nav("/dev/ttyUSB0");
    std::string command = "$VNWRG,06,0*XX\r\n";
    vector_nav.writeSerial(command);
    double start_time = getCurrentTime();
    int count = 0;
    while(getCurrentTime() - start_time < 10000) {
        command = "$VNRRG,08*XX\r\n";
        vector_nav.writeSerial(command);
        std::string str = vector_nav.readSerial();
        //std::cout << "RPY: " << str << std::endl;
        command = "$VNRRG,54*XX\r\n";
        vector_nav.writeSerial(command);
        str = vector_nav.readSerial();
        //std::cout << "IMU: " << str << std::endl;
        count++;
    }
    double freq = count / 10.0;
    std::cout << "DONE " << freq << std::endl;
    return 0;
}