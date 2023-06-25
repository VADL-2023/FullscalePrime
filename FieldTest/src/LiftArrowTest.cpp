/**
 * @file LiftArrowTest.cpp
 * @brief Runs lift up or down
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

/**
 * @brief RPi GPIO Pin for positive lift motor terminal
 * 
 */
const int LIFT_P = 20;

/**
 * @brief RPi GPIO Pin for negative lift motor terminal
 * 
 */
const int LIFT_N = 16;

/**
 * @brief RPi GPIO Pin for lift motor enable
 * 
 */
const int LIFT_ENABLE = 12;

/**
 * @brief RPi GPIO Pin for lift motor controller standby
 * 
 */
const int LIFT_STANDBY = 21;

/**
 * @brief Max PWM Signal for motor to run full speed
 * 
 */
const int PWM_MOTOR_MAX = 255;

/**
 * @brief Main method: Runs lift up or down
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
        // Lift up
        if (userInput == "U")
        {
            gpioWrite(LIFT_STANDBY, 1);
            gpioWrite(LIFT_P, 1);
            gpioWrite(LIFT_N, 0);
            gpioPWM(LIFT_ENABLE, PWM_MOTOR_MAX);
        }
        //Lift down
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