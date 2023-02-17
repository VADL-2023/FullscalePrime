#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>

const int NACELLE_SERVO = 14;
const uint16_t SERVO_PULSE_MIN = 100;
const uint16_t SERVO_PULSE_MAX = 2500;
const uint8_t SERVO_DEG_RANGE = 180;
const int NACELLE_LOCK = 1375;
const int NACELLE_UNLOCK = 2250;

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
    gpioSetMode(NACELLE_SERVO, PI_OUTPUT);

    // Wait for the user to signal the lift servo
    double angle = 0;
    float pulse_width;
    bool run_again = true;
    do
    {

        // Wait for the user to signal that the parachute should be detached
        std::string userInput = "";
        std::cout << "What do you want to do to the lift (L | U | Q): ";
        std::cin >> userInput;

        if (userInput == "L")
        {
            std::cout << "Start lock" << std::endl;
            angle = 0;
            pulse_width = angleToPulseWidth(SERVO_PULSE_MAX, SERVO_PULSE_MIN, SERVO_DEG_RANGE, angle);
            gpioServo(NACELLE_SERVO, NACELLE_LOCK);
            gpioSleep(0, 2, 0);
            std::cout << "End lock" << std::endl;
        }
        else if (userInput == "U")
        {
            std::cout << "Start unlock" << std::endl;
            angle = 90;
            pulse_width = angleToPulseWidth(SERVO_PULSE_MAX, SERVO_PULSE_MIN, SERVO_DEG_RANGE, angle);
            gpioServo(NACELLE_SERVO, NACELLE_UNLOCK);
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
