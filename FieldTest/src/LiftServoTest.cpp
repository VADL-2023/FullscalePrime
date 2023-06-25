
/**
 * @file LiftServoTest.cpp 
 * 
 * @brief Runs lift servo to either lock or unlock the lift
 * WARNING: Change servo parameters since it has been a while and the servo may be in a different configuration
 * 
 */

#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

/**
 * @brief RPI GPIO pin for servo
 * 
 */
const int LIFT_SERVO = 2;

/**
 * @brief Servo pulse width for lock configuration
 * 
 */
const int LIFT_LOCK = 1750;

/**
 * @brief Servo pulse width for unlock configuration
 * 
 */
const int LIFT_UNLOCK = 1300;

/**
 * @brief Main Method: Runs lift servo to either lock or unlock the lift
 * 
 * @return Method status
 */
int main()
{
    // Temporary initialization thing
    while (gpioInitialise() <= 0)
    {
        std::cout << "GPIOs not initialized" << std::endl;
    }

    std::cout << "GPIOs initialized" << std::endl;

    // Lift Locking Servo Initialization
    gpioSetMode(LIFT_SERVO, PI_OUTPUT);

    // Wait for the user to signal the lift servo
    bool run_again = true;
    do
    {

        std::string userInput = "";
        std::cout << "What do you want to do to the lift (L | U | Q): ";
        std::cin >> userInput;

        if (userInput == "L")
        {
            std::cout << "Start lock" << std::endl;
            gpioServo(LIFT_SERVO, LIFT_LOCK);
            gpioSleep(0, 2, 0);
            std::cout << "End lock" << std::endl;
        }
        else if (userInput == "U")
        {
            std::cout << "Start unlock" << std::endl;
            gpioServo(LIFT_SERVO, LIFT_UNLOCK);
            gpioSleep(0, 2, 0);
            std::cout << "End unlock" << std::endl;
        }
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
