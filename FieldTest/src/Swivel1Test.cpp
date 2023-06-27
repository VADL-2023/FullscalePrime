/**
 * @file Swivel1Test.cpp
 * @brief Swivels nacelle 1 left or right
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "Stepper.h"
#include <unistd.h>

/**
 * @brief Number of steps for full revolution for stepper motor
 * 
 */
const int STEPS_PER_REVOLUTION = 200;

/**
 * @brief Speed of stepper motor (unitless)
 * 
 */
const int STEPPER_SPEED = 5;

/**
 * @brief Number of steps run in single key press
 * 
 */
const int NUM_STEPS = 40;

/**
 * @brief RPi GPIO Pin for stepper 1 standby
 * 
 */
const int STEPPER_1_STANDBY_PIN = 0;

/**
 * @brief RPi GPIO Pin for stepper 1 pin 1
 * 
 */
const int STEPPER_1_PIN_1 = 11;

/**
 * @brief RPi GPIO Pin for stepper 1 pin 2
 * 
 */
const int STEPPER_1_PIN_2 = 9;

/**
 * @brief RPi GPIO Pin for stepper 1 pin 3
 * 
 */
const int STEPPER_1_PIN_3 = 5;

/**
 * @brief RPi GPIO Pin for stepper 1 pin 4
 * 
 */
const int STEPPER_1_PIN_4 = 6;

/**
 * @brief Main Method: Swivels nacelle 1 left or right
 * 
 * @return exit status
 */
int main()
{
    // Temporary initialization thing
    while (gpioInitialise() <= 0)
    {
        std::cout << "GPIOs not initialized" << std::endl;
    }

    std::cout << "GPIOs initialized" << std::endl;

    //Initialize stepper 1
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
        // Swivel left
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
        // Swivel Right
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
        // Stop program
        else if (userInput == "S")
        {
            gpioWrite(STEPPER_1_STANDBY_PIN, 0);
            exit = true;
        }
    } while (!exit);

    gpioTerminate();
    return 0;
}
