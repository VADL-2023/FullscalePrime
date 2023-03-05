#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

const int LEVEL_SERVO = 3;
const double MAX_UP_ANGLE = -15;
const double MAX_DOWN_ANGLE = 15;
const int SERVO_UP_POS = 0;
const int SERVO_DOWN_POS = 45;
const uint16_t SERVO_PULSE_MIN = 500;
const uint16_t SERVO_PULSE_MAX = 2250;
const uint8_t SERVO_DEG_RANGE = 90;

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

    // Lift Locking Servo Initialization
    gpioSetMode(LEVEL_SERVO, PI_OUTPUT);

    // Wait for the user to signal the lift servo
    double angle = 0;
    float pulse_width;
    bool run_again = true;
    do
    {

        // Wait for the user to signal that the parachute should be detached
        std::string userInput = "";
        std::cout << "What do you want to do to the lift (U | D | Q): ";
        std::cin >> userInput;

        if (userInput == "U")
        {
            std::cout << "Start up" << std::endl;
            int desired_servo_pos = SERVO_UP_POS + ((MAX_UP_ANGLE - MAX_UP_ANGLE) * (SERVO_DOWN_POS - SERVO_UP_POS)) / (MAX_DOWN_ANGLE - MAX_UP_ANGLE);
            std::cout << "Desired Servo Pos: " << desired_servo_pos << std::endl;
            float pulse_width = angleToPulseWidth(SERVO_PULSE_MAX, SERVO_PULSE_MIN, SERVO_DEG_RANGE, desired_servo_pos);
            std::cout << "Pulse width: " << pulse_width << std::endl;
            gpioServo(LEVEL_SERVO, pulse_width);
            gpioSleep(0, 2, 0);
            std::cout << "End Up" << std::endl;
        }
        else if (userInput == "D")
        {
            std::cout << "Start down" << std::endl;
            int desired_servo_pos = SERVO_UP_POS + ((MAX_DOWN_ANGLE - MAX_UP_ANGLE) * (SERVO_DOWN_POS - SERVO_UP_POS)) / (MAX_DOWN_ANGLE - MAX_UP_ANGLE);
            std::cout << "Desired Servo Pos: " << desired_servo_pos << std::endl;
            float pulse_width = angleToPulseWidth(SERVO_PULSE_MAX, SERVO_PULSE_MIN, SERVO_DEG_RANGE, desired_servo_pos);
            std::cout << "Pulse width: " << pulse_width << std::endl;
            gpioServo(LEVEL_SERVO, pulse_width);
            gpioSleep(0, 2, 0);
            std::cout << "End Down" << std::endl;
        }
        else if (userInput == "Q")
        {
            run_again = false;
            std::cout << "Quitting" << std::endl;
        }
        else
        {
            std::cout << "Please select (U)p, (D)own, or (Q)uit" << std::endl;
        }

    } while (run_again);
    gpioTerminate();
    std::cout << "GPIO terminated" << std::endl;
    return 0;
}
