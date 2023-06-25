/**
 * @file Swivel2Test.cpp
 * @brief Swivels nacelle 2 left or right
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
const int NUM_STEPS = 34;

/**
 * @brief RPi GPIO Pin for stepper 2 standby
 * 
 */
const int STEPPER_2_STANDBY_PIN = 27;

/**
 * @brief RPi GPIO Pin for stepper 2 pin 1
 * 
 */
const int STEPPER_2_PIN_1 = 17;

/**
 * @brief RPi GPIO Pin for stepper 2 pin 2
 * 
 */
const int STEPPER_2_PIN_2 = 4;

/**
 * @brief RPi GPIO Pin for stepper 2 pin 3
 * 
 */
const int STEPPER_2_PIN_3 = 22;

/**
 * @brief RPi GPIO Pin for stepper 2 pin 4
 * 
 */
const int STEPPER_2_PIN_4 = 10;

/**
 * @brief Main Method: Swivels nacelle 2 left or right
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

    //Initialize stepper 2
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
        // Swivel left
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
        // Swivel Right
        else if (userInput == "R")
        {
            gpioWrite(STEPPER_2_STANDBY_PIN, 1);
            usleep(500000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_2.step(-NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(500000);
            gpioWrite(STEPPER_2_STANDBY_PIN, 0);
        }
        // Stop program
        else if (userInput == "S")
        {
            gpioWrite(STEPPER_2_STANDBY_PIN, 0);
            exit = true;
        }
    } while (!exit);

    gpioTerminate();
    return 0;
}
