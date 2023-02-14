#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "Stepper.h"
#include <unistd.h>

const int TIME_THRESHOLD = 20000;
const int STEPS_PER_REVOLUTION = 200;
const int STEPPER_SPEED = 30;
const int NUM_STEPS = 40;
const int STEPPER_2_STANDBY_PIN = 27;
const int STEPPER_2_PIN_1 = 17;
const int STEPPER_2_PIN_2 = 4;
const int STEPPER_2_PIN_3 = 22;
const int STEPPER_2_PIN_4 = 10;

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
    Stepper stepper_2;
    stepper_2.initialize(STEPS_PER_REVOLUTION, STEPPER_2_PIN_1, STEPPER_2_PIN_2, STEPPER_2_PIN_3, STEPPER_2_PIN_4, true);
    gpioSetMode(STEPPER_2_STANDBY_PIN, PI_OUTPUT);
    gpioWrite(STEPPER_2_STANDBY_PIN, 0);
    stepper_2.setSpeed(STEPPER_SPEED);
    bool exit = false;
    do
    {
        std::string userInput = "";
        std::cout << "What do you want to do to the lift ((L)eft | (R)ight | (S)top): ";
        std::cin >> userInput;
        if (userInput == "L")
        {
            gpioWrite(STEPPER_2_STANDBY_PIN, 1);
            usleep(500000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_2.step(NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(500000);
            gpioWrite(STEPPER_2_STANDBY_PIN, 0);
        }
        else if (userInput == "R")
        {
            gpioWrite(STEPPER_2_STANDBY_PIN, 1);
            usleep(1000000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_2.step(-NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(1000000);
            gpioWrite(STEPPER_2_STANDBY_PIN, 0);
        }
        else if (userInput == "S")
        {
            gpioWrite(STEPPER_2_STANDBY_PIN, 0);
            exit = true;
        }
    } while (!exit);

    gpioTerminate();
    return 0;
}