#include "pigpio.h"
#include <iostream>
uint16_t pulseMin = 759; // [usecs] pulse width to send servo to one end of motion range
uint16_t pulseMax = 2250; // [usecs] pulse width to send servo to other end of motion range
uint8_t servoRange = 180; // [deg] possible range of motion of servo
uint8_t servoPinN = 12;

float convertAngle2PulseWidth(float angle){
    return (pulseMax - pulseMin)*angle/servoRange + (pulseMax + pulseMin)/2.0; // angle between +-90
}

void moveServo(uint8_t pin, float angle){
    float pulseWidth = convertAngle2PulseWidth(angle);
    gpioServo(pin, pulseWidth);
}

int main() {
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
    gpioSetMode(servoPinN,PI_OUTPUT);
    float angle = 12;
    moveServo(servoPinN,angle);
    gpioSleep(0,1,0);
    std::cout << "Terminate " << gpioGetServoPulsewidth(servoPinN) << std::endl;
    gpioTerminate();
    return 0;
}