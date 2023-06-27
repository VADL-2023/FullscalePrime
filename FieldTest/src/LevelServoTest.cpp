/**
 * @file LevelServoTest.cpp
 * @brief Runs level servo to tilt the level up or down
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

/**
 * @brief RPI GPIO Pin for Level Servo
 * 
 */
const int LEVEL_SERVO = 3;

/**
 * @brief Main method: Runs level servo to tilt the level up or down
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

    // Wait for the user to signal the lift servo
    bool run_again = true;

    //Initial level pulse width
    int pulseWidth = 1500;
    do
    {

        /**
         * @brief WARNING: It copies lock and unlock from the other servo test, but it just means tilt up or down
         * 
         */
        std::string userInput = "";
        std::cout << "What do you want to do to the lift (L | U | Q): ";
        std::cin >> userInput;
        
        // Tilt one way (sorry, I forgot which way)
        if (userInput == "L")
        {
            pulseWidth += 50;
            std::cout << "Pulse width: " << pulseWidth << std::endl;
            gpioServo(LEVEL_SERVO, pulseWidth);
            gpioSleep(0, 2, 0);
        }
        //Tilt the other way
        else if (userInput == "U")
        {
            pulseWidth -= 50;
            std::cout << "Pulse width: " << pulseWidth << std::endl;
            gpioServo(LEVEL_SERVO, pulseWidth);
            gpioSleep(0, 2, 0);
        }
        // Quit
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
