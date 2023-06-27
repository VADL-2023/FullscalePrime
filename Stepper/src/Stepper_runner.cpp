#include <iostream>
#include "Stepper.h"
#include "pigpio.h"
/**
 * @file Stepper_runner.cpp
 * @brief Tests stepper motor
 * 
 */
#include <chrono>
#include <ctime>
#include <unistd.h>

/**
 * @brief Get the current time in ms
 * 
 * @return double representing the current time
 */
double getCurrentTime() {
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

/**
 * @brief Main Method to test individual stepper motor
 * 
 * @return int representing exit status
 */
int main() {
    double startTime = getCurrentTime();
    int standby_pin = 0;
    int steps_per_revolution = 200;
    while(gpioInitialise() <= 0) {
        std::cout << "Not initialized" << std::endl;
    }
    std::cout << "Initialized" << std::endl;
    gpioSetMode(standby_pin,PI_OUTPUT);
    //Standby pin
    gpioWrite(standby_pin,0);
    //std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
    Stepper the_stepper(steps_per_revolution,11,9,5,6,true);
    the_stepper.setSpeed(30);
    while(getCurrentTime() - startTime < 10000) {
        gpioWrite(standby_pin,1);
        std::cout << "Moving one way" << std::endl;
        //std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
        the_stepper.step(40);
        std::cout << "Pausing" << std::endl;
        gpioWrite(standby_pin,0);
        usleep(2000000);
        gpioWrite(standby_pin,1);
        std::cout << "Moving other way" << std::endl;
        the_stepper.step(-40);
        std::cout << "Pausing" << std::endl;
        gpioWrite(standby_pin,0);
        usleep(2000000);
    }
    gpioTerminate();
    return 0;
}