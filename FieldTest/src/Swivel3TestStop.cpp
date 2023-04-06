#include <iostream>
#include <ctime>
#include "pigpio.h"
#include <chrono>
#include "Stepper.h"
#include <unistd.h>
#include <ncurses.h>

const int TIME_THRESHOLD = 20000;
const int STEPS_PER_REVOLUTION = 200;
const int STEPPER_SPEED = 10;
const int NUM_STEPS = 60;
const int STEPPER_3_STANDBY_PIN = 8;
const int STEPPER_3_PIN_1 = 7;
const int STEPPER_3_PIN_2 = 1;
const int STEPPER_3_PIN_3 = 25;
const int STEPPER_3_PIN_4 = 24;

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
    Stepper stepper_3;
    stepper_3.initialize(STEPS_PER_REVOLUTION, STEPPER_3_PIN_1, STEPPER_3_PIN_2, STEPPER_3_PIN_3, STEPPER_3_PIN_4, true);
    gpioSetMode(STEPPER_3_STANDBY_PIN, PI_OUTPUT);
    gpioWrite(STEPPER_3_STANDBY_PIN, 0);
    gpioTerminate();
    std::cout << "Terminated" << std::endl;
    return 0;
    stepper_3.setSpeed(STEPPER_SPEED);
    bool exit = false;
    char c;
    while(true) {
        gpioWrite(STEPPER_3_STANDBY_PIN,1);
        stepper_3.step(NUM_STEPS);
        c = getch();
        std::cout << "Char: " << c << std::endl;
        if(c == 'e') {
            break;
        }
        usleep(1000000);
        stepper_3.step(-NUM_STEPS);
        c = getch();
        std::cout << "Char: " << c << std::endl;
        if(c == 'e') {
            break;
        }
        usleep(1000000);
        stepper_3.step(-NUM_STEPS);
        c = getch();
        std::cout << "Char: " << c << std::endl;
        if(c == 'e') {
            break;
        }
        usleep(1000000);
        stepper_3.step(NUM_STEPS);
        c = getch();
        std::cout << "Char: " << c << std::endl;
        if(c == 'e') {
            break;
        }
        usleep(1000000);
        gpioWrite(STEPPER_3_STANDBY_PIN,0);
        usleep(10000000);
    }
    gpioWrite(STEPPER_3_STANDBY_PIN,0);
    /*do
    {
        std::string userInput = "";
        std::cout << "What do you want to do to the lift ((L)eft | (R)ight | (S)top): ";
        std::cin >> userInput;
        if (userInput == "L")
        {
            gpioWrite(STEPPER_3_STANDBY_PIN, 1);
            usleep(1000000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_3.step(NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(1000000);
            gpioWrite(STEPPER_3_STANDBY_PIN, 0);
        }
        else if (userInput == "R")
        {
            gpioWrite(STEPPER_3_STANDBY_PIN, 1);
            usleep(1000000);
            std::cout << "Moving one way" << std::endl;
            // std::cout << "Standby: " << gpioRead(standby_pin) << std::endl;
            stepper_3.step(-NUM_STEPS);
            std::cout << "Pausing" << std::endl;
            usleep(1000000);
            gpioWrite(STEPPER_3_STANDBY_PIN, 0);
        }
        else if (userInput == "S")
        {
            gpioWrite(STEPPER_3_STANDBY_PIN, 0);
            exit = true;
        }
    } while (!exit);*/

    gpioTerminate();
    return 0;
}
