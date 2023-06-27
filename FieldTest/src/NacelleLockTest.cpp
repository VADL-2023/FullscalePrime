/**
 * @file NacelleLockTest.cpp
 * @brief Runs nacelle locking servo to either lock or unlock the nacelles
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

/**
 * @brief RPI GPIO pin for nacelle locking servo
 * 
 */
const int NACELLE_SERVO = 14;

/**
 * @brief Pulse width to lock nacelles
 * 
 */
const int NACELLE_LOCK = 1600;

/**
 * @brief Pulse width to unlock nacelles
 * 
 */
const int NACELLE_UNLOCK = 755;

/**
 * @brief Main Method: Runs nacelle locking servo to either lock or unlock the nacelles
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

    // Nacelle Locking Servo Initialization
    gpioSetMode(NACELLE_SERVO, PI_OUTPUT);

    // Wait for the user to signal the nacelle servo
    bool run_again = true;
    do
    {

        // Wait for the user to signal that the parachute should be detached
        std::string userInput = "";
        std::cout << "What do you want to do to the nacelle lock (L | U | Q): ";
        std::cin >> userInput;

        // Lock nacelle servo
        if (userInput == "L")
        {
            std::cout << "Start lock" << std::endl;
            gpioServo(NACELLE_SERVO, NACELLE_LOCK);
            gpioSleep(0, 2, 0);
            std::cout << "End lock" << std::endl;
        }
        // Unlock nacelle servo
        else if (userInput == "U")
        {
            std::cout << "Start unlock" << std::endl;
            gpioServo(NACELLE_SERVO, NACELLE_UNLOCK);
            gpioSleep(0, 2, 0);
            std::cout << "End unlock" << std::endl;
        }
        // Quit program
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
