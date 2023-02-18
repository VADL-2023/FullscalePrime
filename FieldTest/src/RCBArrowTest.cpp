#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "sensors.h"
#include "config_IMU.hpp" //Copy this file into MainDriver includes -> currently in IMU

const int TIME_THRESHOLD = 20000;

const int RCB_P = 26;
const int RCB_N = 19;
const int RCB_ENABLE = 13;
const int RCB_STANDBY = 21;
const int PWM_MOTOR_MAX = 255;

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

    // Lift Motor Initialization
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
        if (userInput == "L")
        {
            gpioWrite(RCB_STANDBY, 1);
            gpioWrite(RCB_P, 1);
            gpioWrite(RCB_N, 0);
            gpioPWM(RCB_ENABLE, PWM_MOTOR_MAX);
            auto responseRollPitchYaw = mVN->readYawPitchRoll();
            std::cout << "Roll: " << responseRollPitchYaw[0] << " Pitch: " << responseRollPitchYaw[1] << " Yaw: " << responseRollPitchYaw[2] << std::endl;
        }
        else if (userInput == "R")
        {
            gpioWrite(RCB_STANDBY, 1);
            gpioWrite(RCB_P, 0);
            gpioWrite(RCB_N, 1);
            gpioPWM(RCB_ENABLE, PWM_MOTOR_MAX);
            auto responseRollPitchYaw = mVN->readYawPitchRoll();
            std::cout << "Roll: " << responseRollPitchYaw[0] << " Pitch: " << responseRollPitchYaw[1] << " Yaw: " << responseRollPitchYaw[2] << std::endl;
        }
        else if (userInput == "S")
        {
            exit = true;
        }
    } while (!exit);
    gpioWrite(RCB_STANDBY, 0);
    gpioWrite(RCB_N, 0);
    gpioWrite(RCB_P, 0);
    gpioPWM(RCB_ENABLE, 0);
    gpioTerminate();
    if (IMU_ACTIVE)
    {
        std::cout << "IMU: Disconnecting" << std::endl;

        mVN->disconnect();

        std::cout << "IMU: Disconnected" << std::endl;
    }

    delete mVN;
    return 0;
}