#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "Stepper.h"
#include <unistd.h>

const int TIME_THRESHOLD = 20000;
const int STEPS_PER_REVOLUTION = 200;
const int STEPPER_SPEED = 5;
const int NUM_STEPS = 40;
const int STEPPER_1_STANDBY_PIN = 0;
const int STEPPER_1_PIN_1 = 11;
const int STEPPER_1_PIN_2 = 9;
const int STEPPER_1_PIN_3 = 5;
const int STEPPER_1_PIN_4 = 6;

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
    Stepper stepper_1;
    stepper_1.initialize(STEPS_PER_REVOLUTION, STEPPER_1_PIN_1, STEPPER_1_PIN_2, STEPPER_1_PIN_3, STEPPER_1_PIN_4, true);
    gpioSetMode(STEPPER_1_STANDBY_PIN, PI_OUTPUT);
    gpioWrite(STEPPER_1_STANDBY_PIN, 0);
    stepper_1.setSpeed(STEPPER_SPEED);
    bool exit = false;
    do
    {
        std::string userInput = "";
        std::cout << "What do you want to do to the lift ((L)eft | (R)ight | (S)top): ";
        std::cin >> userInput;
        if (userInput == "L")
        {
            gpioWrite(STEPPER_1_STANDBY_PIN, 1);
            usleep(500000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_1.step(NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(500000);
            gpioWrite(STEPPER_1_STANDBY_PIN, 0);
        }
        else if (userInput == "R")
        {
            gpioWrite(STEPPER_1_STANDBY_PIN, 1);
            usleep(500000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_1.step(-NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(500000);
            gpioWrite(STEPPER_1_STANDBY_PIN, 0);
        }
        else if (userInput == "S")
        {
            gpioWrite(STEPPER_1_STANDBY_PIN, 0);
            exit = true;
        }
    } while (!exit);

    gpioTerminate();
    return 0;
}
