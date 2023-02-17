#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

const int TIME_THRESHOLD = 20000;

const int LIFT_P = 20;
const int LIFT_N = 16;
const int LIFT_ENABLE = 12;
const int LIFT_STANDBY = 21;
const int PWM_MOTOR_MAX = 255;

double getCurrentTime()
{
    return double(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}

float angleToPulseWidth(double pulse_max, double pulse_min, double range, float angle)
{
    return (pulse_max - pulse_min) * angle / range + (pulse_max + pulse_min) / 2.0;
}

int main()
{
    // Temporary initialization thing
    while (gpioInitialise() <= 0)
    {
        std::cout << "GPIOs not initialized" << std::endl;
    }

    std::cout << "GPIOs initialized" << std::endl;

    // Lift Motor Initialization
    gpioSetMode(LIFT_P, PI_OUTPUT);
    gpioSetMode(LIFT_N, PI_OUTPUT);
    gpioSetMode(LIFT_STANDBY, PI_OUTPUT);
    gpioWrite(LIFT_STANDBY, 0);
    gpioSetMode(LIFT_ENABLE, PI_OUTPUT);
    gpioPWM(LIFT_ENABLE, 0);
    bool exit = false;
    do
    {
        std::string userInput = "";
        std::cout << "What do you want to do to the lift ((U)p | (D)own | (S)top): ";
        std::cin >> userInput;
        if (userInput == "U")
        {
            gpioWrite(LIFT_STANDBY, 1);
            gpioWrite(LIFT_P, 1);
            gpioWrite(LIFT_N, 0);
            gpioPWM(LIFT_ENABLE, PWM_MOTOR_MAX);
        }
        else if (userInput == "D")
        {
            gpioWrite(LIFT_STANDBY, 1);
            gpioWrite(LIFT_P, 0);
            gpioWrite(LIFT_N, 1);
            gpioPWM(LIFT_ENABLE, PWM_MOTOR_MAX);
        }
        else if (userInput == "S")
        {
            exit = true;
        }
    } while (!exit);
    gpioWrite(LIFT_STANDBY, 0);
    gpioWrite(LIFT_N, 0);
    gpioWrite(LIFT_P, 0);
    gpioPWM(LIFT_ENABLE, 0);
    std::cout << "Winch down" << std::endl;

    gpioTerminate();
    return 0;
}