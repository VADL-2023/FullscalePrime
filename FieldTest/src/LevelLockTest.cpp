/**
 * @file LevelLockTest.cpp
 * @brief Locks the level at its default configuration
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

/**
 * @brief RPI GPIO pin for level servo
 * 
 */
const int LEVEL_SERVO = 3;

/**
 * @brief Main Method: Locks the level at its default configuration
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

    // Level Servo Initialization
    gpioSetMode(LEVEL_SERVO, PI_OUTPUT);

    // Wait for the user to signal the level servo
    bool run_again = true;
    int pulseWidth = 1450;
    do
    {

        std::string userInput = "";
        std::cout << "What do you want to do to the lift (L | Q): ";
        std::cin >> userInput;
        
        // Lock level servo at default configuration
        if (userInput == "L")
        {
            std::cout << "Pulse width: " << pulseWidth << std::endl;
            gpioServo(LEVEL_SERVO, pulseWidth);
            gpioSleep(0, 2, 0);
        }
        //Quit out of program
        else if (userInput == "Q")
        {
            run_again = false;
            std::cout << "Quitting" << std::endl;
        } else {
            std::cout << "Please select (L)ock, (U)nlock, or (Q)uit" << std::endl;
        }

    } while (run_again);
    gpioTerminate();
    std::cout << "GPIO terminated" << std::endl;
    return 0;
}
