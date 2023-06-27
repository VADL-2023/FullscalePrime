/**
 * @file SerialTest.cpp
 * @brief Simple test of Serial Object
 * 
 */
#include <string>
#include <iostream>
#include <unistd.h>
#include "SerialObject.h"

/**
 * @brief Main method to test Serial communication with Arduino/Teensy
 * 
 * @return int representing exit status
 */
int main() {
    SerialObject serial;

    // Prints to the Teensy
    serial.writeSerial("Hello, World!\n");

    std::cout << "Waiting for input" << std::endl;

    std::string str;
    do {
        sleep(1);
        //Read from the teensy
        str = serial.readSerial();
    } while(str.empty());

    std::cout << str << std::endl;

    return 0;
}