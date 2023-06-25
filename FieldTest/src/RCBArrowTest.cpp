/**
 * @file RCBArrowTest.cpp
 * @brief Runs RCB Motor clockwise or counterclockwise
 * 
 */
#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "sensors.h"
#include "config_IMU.hpp" //Copy this file into MainDriver includes -> currently in IMU

/**
 * @brief RPi GPIO Pin for positive RCB motor terminal
 * 
 */
const int RCB_P = 26;

/**
 * @brief RPi GPIO Pin for negative RCB motor terminal
 * 
 */
const int RCB_N = 19;

/**
 * @brief RPi GPIO Pin for RCB motor enable
 * 
 */
const int RCB_ENABLE = 13;

/**
 * @brief RPi GPIO Pin for RCB motor controller standby
 * 
 */
const int RCB_STANDBY = 21;

/**
 * @brief Max PWM Signal for motor to run full speed
 * 
 */
const int PWM_MOTOR_MAX = 255;

/**
 * @brief Main Method: Runs RCB Motor clockwise or counterclockwise
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

    //Initialize VN-100
    VnSensor *mVN = new VnSensor();
    std::cout << "IMU Connecting" << std::endl;
    mVN->connect(IMU_PORT, IMU_BAUD_RATE);
    if (!mVN->isConnected())
    {
        throw "IMU Failed to Connect";
    }
    else
    {
        std::cout << "IMU Connected\n"
                  << std::endl;
    }

    // RCB Motor Initialization
    gpioSetMode(RCB_P, PI_OUTPUT);
    gpioSetMode(RCB_N, PI_OUTPUT);
    gpioSetMode(RCB_STANDBY, PI_OUTPUT);
    gpioWrite(RCB_STANDBY, 0);
    gpioSetMode(RCB_ENABLE, PI_OUTPUT);
    gpioPWM(RCB_ENABLE, 0);
    bool exit = false;
    do
    {
        std::string userInput = "";
        std::cout << "What do you want to do to the lift ((L)eft | (R)ight | (S)top): ";
        std::cin >> userInput;
        //  Rotate Left
        if (userInput == "L")
        {
            gpioWrite(RCB_STANDBY, 1);
            gpioWrite(RCB_P, 1);
            gpioWrite(RCB_N, 0);
            gpioPWM(RCB_ENABLE, PWM_MOTOR_MAX);
            auto responseRollPitchYaw = mVN->readYawPitchRoll();
            std::cout << "Roll: " << responseRollPitchYaw[0] << " Pitch: " << responseRollPitchYaw[1] << " Yaw: " << responseRollPitchYaw[2] << std::endl;
        }
        // Rotate Right
        else if (userInput == "R")
        {
            gpioWrite(RCB_STANDBY, 1);
            gpioWrite(RCB_P, 0);
            gpioWrite(RCB_N, 1);
            gpioPWM(RCB_ENABLE, PWM_MOTOR_MAX);
            auto responseRollPitchYaw = mVN->readYawPitchRoll();
            std::cout << "Roll: " << responseRollPitchYaw[0] << " Pitch: " << responseRollPitchYaw[1] << " Yaw: " << responseRollPitchYaw[2] << std::endl;
        }
        // Stop
        else if (userInput == "S")
        {
            exit = true;
        }
    } while (!exit);
    // Stop motor
    gpioWrite(RCB_STANDBY, 0);
    gpioWrite(RCB_N, 0);
    gpioWrite(RCB_P, 0);
    gpioPWM(RCB_ENABLE, 0);
    gpioTerminate();

    // Disconnect IMU
    if (IMU_ACTIVE)
    {
        std::cout << "IMU: Disconnecting" << std::endl;

        mVN->disconnect();

        std::cout << "IMU: Disconnected" << std::endl;
    }

    delete mVN;
    return 0;
}